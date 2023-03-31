#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <time.h>

#include "cpu_thread.h"
#include "io_thread.h"
#include "file_parser.h"
#include "double_linked_list.h"
#include "process.h"
#include "options.h"

#define SHM_SIZE sizeof(Process) // shared memory size

sem_t thread_access; // allow threads to start running and communicate when done

// mutexs to handle access to shared queues
pthread_mutex_t readyq_mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t ioq_mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t completeq_mtx = PTHREAD_MUTEX_INITIALIZER;

// mutex for access to proccess count integer
pthread_mutex_t proc_count_mtx = PTHREAD_MUTEX_INITIALIZER;

// mutex to handle starting of threads
pthread_mutex_t thread_running_mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t thread_running_cond = PTHREAD_COND_INITIALIZER;
int fp_done = 0;

int main(int argc, char const *argv[]) {
    
    ////////////////////////////////////////////////
    // Get input from CLI arguments
    ////////////////////////////////////////////////
    
    // set up variables for CLI input
    int algo = 0;
    int quantum = 0;
    FILE *file_ptr;
    char file_name[128];
    char alg_type[32];

    // step through given arguments
    for(int i = 1; i < argc; i++){
        // algorithm option
        if( strcmp(argv[i], "-alg") == 0 ){
            // check next argument for algorithm to use
            strcpy(alg_type, argv[i+1]);
            if (strcmp(argv[i+1], "FCFS") == 0) {
                if(DEBUG) { printf("Activating FIRST COME FIRST SERVE\n"); }
                algo = 1;
            }
            else if (strcmp(argv[i+1], "SJF") == 0) {
                if(DEBUG) { printf("Activating SHORTEST JOB FIRST\n"); }
                algo = 2;
            }
            else if (strcmp(argv[i+1], "PR") == 0) {
                if(DEBUG) { printf("Activating PRIORITY SCHEDULING\n"); }
                algo = 3;
            }
            else if (strcmp(argv[i+1], "RR") == 0) {
                if(DEBUG) { printf("Activating ROUND ROBIN\n"); }
                algo = 4;
            }
            else if (strcmp(argv[i+1], "FCLS") == 0) {
                if(DEBUG) { printf("Activating FIRST COME LAST SERVE\n"); }
                algo = 5;
            }
            else if (strcmp(argv[i+1], "FCRS") == 0) {
                if(DEBUG) { printf("Activating FIRST COME RANDOM SERVE\n"); }
                algo = 6;
            }
            else {
                if(DEBUG) { fprintf(stderr, "Error: Invalid algorithm\n"); }
                exit(1);
            }
            i++;
        }

        // quantum option
        if (strcmp(argv[i], "-quantum") == 0 && algo == 4 ) {
            // check next argument for quantum size
            if(DEBUG) { printf("Setting Quantum\n"); }
            quantum = atoi(argv[++i]);
        }

        // input option
        if (strcmp(argv[i], "-input") == 0) {
            strcpy(file_name, argv[i+1]);
            // check next argument for file_name
            if(DEBUG) { printf("ATTEMPTING TO OPEN FILE %s\n", argv[i+1]); }
            file_ptr = fopen(argv[i+1], "r");
            i++;

            // check if file was opened
            if (file_ptr == NULL) {
                fprintf(stderr, "Error: Cannot open file\n");
                exit(1);
            }
        }
    }

    // check if we got an algorithm and input at least
    if (!algo) {
        fprintf(stderr, "Error: Missing algorithm\n");
        printf("Usage: ./scheduler -alg [FCFS,SJF,PR,RR] [-quantum [integer(ms)] -input [filename]\n");
        exit(1);
    }
    else if (!file_ptr) {
        fprintf(stderr, "Error: Missing input file\n");
        printf("Usage: ./scheduler -alg [FCFS,SJF,PR,RR] [-quantum [integer(ms)] -input [filename]\n");
        exit(1);
    }
    else if (algo == 4 && !quantum) {
        fprintf(stderr, "Error: Missing quantum time\n");
        printf("Usage: ./scheduler -alg [FCFS,SJF,PR,RR] [-quantum [integer(ms)] -input [filename]\n");
        exit(1);
    }

    ///////////////////////////////////////////////////
    // End input
    ///////////////////////////////////////////////////

    // set up semaphore
    // initialize to 0 to synchronize threads running
    sem_init(&thread_access, 0, 0);

    // create shared memory ids
    int shm_readyq_id = shmget(IPC_PRIVATE, sizeof(DoublyLinkedList), IPC_CREAT | 0666);
    if (shm_readyq_id < 0) {
        perror("shmget shm_readyq_id\n");
        exit(1);
    }

    int shm_ioq_id = shmget(IPC_PRIVATE, sizeof(DoublyLinkedList), IPC_CREAT | 0666);
    if (shm_ioq_id < 0) {
        perror("shmget shm_ioq_id\n");
        exit(1);
    }

    int shm_completeq_id = shmget(IPC_PRIVATE, sizeof(DoublyLinkedList), IPC_CREAT | 0666);
    if (shm_completeq_id < 0) {
        perror("shmget shm_completeq_id\n");
        exit(1);
    }

    int shm_proc_count_id = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
    if (shm_proc_count_id < 0) {
        perror("shmget shm_proc_count_id\n");
        exit(1);
    }
 

    // create shared memory pointers for various queues
    DoublyLinkedList *ready_queue = (DoublyLinkedList*) shmat(shm_readyq_id, NULL, 0);
    if (ready_queue == (void*) -1) {
        perror("shmat ready_queue\n");
        exit(1);
    }

    DoublyLinkedList *io_queue = (DoublyLinkedList*) shmat(shm_ioq_id, NULL, 0);
    if (io_queue == (void*) -1) {
        perror("shmat io_queue\n");
        exit(1);
    }

    DoublyLinkedList *complete_queue = (DoublyLinkedList*) shmat(shm_completeq_id, NULL, 0);
    if (complete_queue == (void*) -1) {
        perror("shmat complete_queue\n");
        exit(1);
    }

    // create shared integer pointer
    int *proc_count = (int*) shmat(shm_proc_count_id, NULL, 0);
    if (proc_count == (void*) -1) {
        perror("shmat proc_count\n");
        exit(1);
    }

    // create lists in the shared memory
    ready_queue = create_list();
    io_queue = create_list();
    complete_queue = create_list();

    // create shared integer in shared memory
    proc_count = (int *) calloc(1, sizeof(int)); 


    pthread_mutex_lock(&thread_running_mtx);

    // create io thread
    pthread_t io_thread;
    io_args_t io_args = { io_queue, ready_queue, complete_queue, proc_count };
    if (pthread_create(&io_thread, NULL, startIO, &io_args) != 0){
        // check if thread was successfully created
        fprintf(stderr, "Error creating IO_thread\n");
        exit(1);
    }

    // create thread for cpu scheduler
    pthread_t cpu_thread;
    CPU_args_t cpu_args = { algo, quantum, ready_queue, io_queue, complete_queue, proc_count}; 
    if (pthread_create(&cpu_thread, NULL, start_scheduler, &cpu_args) != 0) {
        // check if thread was successfully created
        fprintf(stderr, "Error creating CPU_thread\n");
        exit(1);
    };

    sem_post(&thread_access);
    sem_post(&thread_access);

    // create input file parsing thread
    pthread_t file_parser;
    Parser_args_t parser_args = { file_ptr, ready_queue, proc_count};
    if (pthread_create(&file_parser, NULL, parse_file, &parser_args) != 0) {
        // check if thread was successfully created
        fprintf(stderr, "Error creating file_parser thread\n");
        exit(1);
    }

    // Get start time
    struct timeval total_start;
    gettimeofday(&total_start, NULL); 

    // set semaphore to 3 to allow threads to start running
    sem_post(&thread_access);

    // join threads and close file
    pthread_join(file_parser, NULL);
    pthread_join(io_thread, NULL);
    pthread_join(cpu_thread, NULL);

    // Get start time
    struct timeval total_end;
    gettimeofday(&total_end, NULL); 

    fclose(file_ptr);

    // calculate results
    struct timeval total_runtime = timeval_diff(total_start, total_end);
    float total_runtime_ms = ((float)total_runtime.tv_sec) * 1000 + ((float)total_runtime.tv_usec) / 1000;
    
    //throughput
    float throughput = complete_queue->size / total_runtime_ms;
    
    //turn around time
    struct timeval total_turnaround = {0, 0};

    struct timeval end;
    struct timeval start;
    struct timeval t_time;

    Node *cursor = complete_queue->head;
    while (cursor) {
        end = cursor->proc->end;
        start = cursor->proc->start;

        t_time = timeval_diff(start, end);

        total_turnaround = timeval_add(total_turnaround, t_time);

        cursor = cursor->next;
    }

    //float total_turnaround_ms = ((float)total_turnaround.tv_sec) * 1000 + ((float)total_turnaround.tv_usec) / 1000;
    int total_turnaround_ms = getTimeInMs(total_turnaround);
    int avg_turnaround = total_turnaround_ms / complete_queue->size;

    //wait time
    struct timeval total_wait = {0, 0};
    cursor = complete_queue->head;
    while (cursor) {
        total_wait = timeval_add(total_wait, cursor->proc->wait);
        cursor = cursor->next;
    }

    int total_wait_ms = getTimeInMs(total_wait);
    if(DEBUG) {printf("Total Wait Time: %d\n", total_wait_ms);}
    int avg_wait = total_wait_ms / complete_queue->size;

    
    // print results
    printf("Input File Name                 : %s\n", file_name);
    printf("CPU Scheduling Alg              : %s ", alg_type);
    if (quantum > 0 && algo == 4) {
        printf("(%d)", quantum);
    }
    printf("\n");
    printf("Total Run Time                  : %d\n", (int)total_runtime_ms);
    printf("Throughput                      : %.3f\n", throughput);
    printf("Avg. Turnaround Time            : %d\n", avg_turnaround);
    printf("Avg. Waiting Time in Ready Queue: %d\n", avg_wait);
    
    // disconnect shared memory
    shmdt(ready_queue);
    shmdt(io_queue);
    shmdt(complete_queue);

    shmctl(shm_readyq_id, IPC_RMID, NULL);
    shmctl(shm_ioq_id, IPC_RMID, NULL);
    shmctl(shm_completeq_id, IPC_RMID, NULL);

    return 0;
}