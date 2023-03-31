#include "cpu_thread.h"


// when we get created
// create ready_queue

// while ready queue has processes
    // check algo type
    // if algo = 1 FCFS
        // pop first
    // if algo = 2
        // pop shortest time
    // if algo = 3
        // pop highest priority
    // if algo = 4
        // pop first process
        // if current_time > quantum
            // sleep for quantum
            // subtract quantum from time
            // add back to ready_queue
        // if current_time <= quantum
            // sleep for time
            // increment index
            // push to io_queue

// start the cpu scheduler
void *start_scheduler(void *arg) {
    // check the algorithm passed
    CPU_args_t *cpu_args = (CPU_args_t *)arg;
    int algo = cpu_args->algo;
    int quantum = cpu_args->quantum;
    DoublyLinkedList *ready_queue = cpu_args->ready_queue;
    DoublyLinkedList *io_queue = cpu_args->io_queue;
    DoublyLinkedList *complete_queue = cpu_args->complete_queue;
    int *proc_count = cpu_args->proc_count;

    if(DEBUG){
        printf("Created CPU Thread\n");
        printf("Algorithm is %d\n", algo); //DEBUG
        printf("Quantum is %d\n", quantum);
    }

    // wait for main to allow us to continue
    sem_wait(&thread_access);
    
    while (1) {
        // check if there is anything in the ready queue
        if (isEmpty(ready_queue)){
            // printf("ready Q is empty\n");
            int sem_value;
            sem_getvalue(&thread_access, &sem_value);
            if (complete_queue->size == *proc_count && sem_value == 2 && fp_done == 1) {
                break;
            }
            continue;
        }

        Process *next_proc;

        pthread_mutex_lock(&readyq_mtx);                    //Lock ready queue
        
        //Check what algorithm we should follow for popping off of the DLL
        switch (algo) {
            case 1: //First Come First Serve
                next_proc = popFirst(ready_queue);
                break;
            case 2: //Shortest Job First
                next_proc = popLeastTimeIndv(ready_queue);
                break;
            case 3: //Priority Scheduling
                next_proc = popHighP(ready_queue);
                break; 
            case 4: //Round Robin
                next_proc = popFirst(ready_queue);
                break;
            case 5: //First Come Last Serve
                next_proc = popLast(ready_queue);
                break;
        }
        pthread_mutex_unlock(&readyq_mtx);                  //Unlock ready queue
        
        if(DEBUG){
            printf("\nProcess popped (cpu)\n");
            print_process(next_proc);
        }

        if (algo == 4 && get_burst_time(next_proc) > quantum){
            // sleep for quantum
            // set up struct for nanosleep
            struct timespec sleep_time = {0, (quantum*1000000)};
            struct timespec remaining_time = {0,1};

            if(DEBUG) {printf("\nCPU: Sleep proc %d for %d\n", next_proc->priority, get_burst_time(next_proc));}
            nanosleep(&sleep_time, &remaining_time);
            next_proc->burst_times[next_proc->index] -= quantum;

            // push it onto the io queue 
            pthread_mutex_lock(&readyq_mtx);
            append(ready_queue, next_proc);
            pthread_mutex_unlock(&readyq_mtx);

        } else {
            // sleep for its time
            // set up struct for nanosleep
            struct timespec sleep_time = {0, (get_burst_time(next_proc)*1000000)};
            struct timespec remaining_time = {0,1};

            if(DEBUG) {printf("\nCPU: Sleep proc %d for %d\n", next_proc->priority, get_burst_time(next_proc));}
            nanosleep(&sleep_time, &remaining_time);
            
            next_proc->index++;

            if (next_proc->index >= next_proc->burst_count - 1) {

                struct timeval end_time;
                gettimeofday(&end_time, NULL);
                next_proc->end = end_time;

                pthread_mutex_lock(&completeq_mtx);
                append(complete_queue, next_proc);
                pthread_mutex_unlock(&completeq_mtx);
                if(DEBUG) {printf("\nProcess %d complete: Adding to complete queue\n", next_proc->priority);}
                continue;
            }

            // push it onto the io queue 
            pthread_mutex_lock(&ioq_mtx);
            append(io_queue, next_proc);
            pthread_mutex_unlock(&ioq_mtx);
        }
    }

    if(DEBUG) {printf("Exiting CPU Thread\n"); }
    sem_post(&thread_access);
    return NULL;
}