#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "cpu_thread.h"
#include "io_thread.h"
#include "file_parser.h"
#include "process.h"

#define DEBUG 1 // toggle for debug purposes
#define SHM_SIZE sizeof(Process) // shared memory size

int parse_cpu_pipe[2];
int cpu_io_pipe[2];

pthread_mutex_t readyq_mtx = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char const *argv[]) {
    
    ////////////////////////////////////////////////
    // Get input from CLI arguments
    ////////////////////////////////////////////////
    
    // set up variables for CLI input
    int algo = 0;
    int quantum = 0;
    FILE *file_ptr;

    // step through given arguments
    for(int i = 1; i < argc; i++){
        // algorithm option
        if( strcmp(argv[i], "-alg") == 0 ){
            // check next argument for algorithm to use
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

    // create shared memory id for various queues
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

    // create lists in the shared memory
    ready_queue = create_list();
    io_queue = create_list();
    complete_queue = create_list();

    // create input file parsing thread
    pthread_t file_parser;
    Parser_args_t parser_args = { file_ptr, ready_queue };
    if (pthread_create(&file_parser, NULL, parse_file, &parser_args) != 0) {
        // check if thread was successfully created
        fprintf(stderr, "Error creating file_parser thread\n");
        exit(1);
    }

    // create thread for cpu scheduler
    pthread_t cpu_thread;
    CPU_args_t cpu_args = { algo, quantum, ready_queue, io_queue, complete_queue }; 
    if (pthread_create(&cpu_thread, NULL, start_scheduler, &cpu_args) != 0) {
        // check if thread was successfully created
        fprintf(stderr, "Error creating CPU_thread\n");
        exit(1);
    };

    // create io thread
    pthread_t io_thread;
    io_args_t io_args = { io_queue, ready_queue };
    if (pthread_create(&io_thread, NULL, startIO, &io_args) != 0){
        // check if thread was successfully created
        fprintf(stderr, "Error creating IO_thread\n");
        exit(1);
    }

    // join threads and close file
    pthread_join(file_parser, NULL);
    pthread_join(io_thread, NULL);
    pthread_join(cpu_thread, NULL);
    fclose(file_ptr);


    // print results
    
    // disconnect shared memory
    shmdt(ready_queue);
    shmdt(io_queue);
    shmdt(complete_queue);

    shmctl(shm_readyq_id, IPC_RMID, NULL);
    shmctl(shm_ioq_id, IPC_RMID, NULL);
    shmctl(shm_completeq_id, IPC_RMID, NULL);

    return 0;
}