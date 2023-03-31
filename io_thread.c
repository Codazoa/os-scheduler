#include "io_thread.h"
        
// when we get created
// grab our shared IO queue

// while things are in our queue
    // pull first process from queue
    // sleep for it's time
    // append to shared ready_queue
    // send back through pipe

// when we recieve EOF
    // exit our thread


//Start running the IO thread
void *startIO(void *arg) {
    // grab arguments
    io_args_t *io_args = (io_args_t *) arg;
    DoublyLinkedList *io_queue = io_args->io_queue;
    DoublyLinkedList *ready_queue = io_args->ready_queue;
    DoublyLinkedList *complete_queue = io_args->complete_queue;
    int *proc_count = io_args->proc_count;

    Process *proc; // variable used to hold current process

    if(DEBUG){
        printf("Created IO Thread\n");
    }

    // wait for main to allow us to continue
    sem_wait(&thread_access);

    while(1){

        if(isEmpty(io_queue)){
            int sem_value;
            sem_getvalue(&thread_access, &sem_value);

            // if all jobs are in complete queue and
            // one other thread has finished and
            // the mutex was able to be locked
            if (complete_queue->size == *proc_count 
                    && sem_value == 1 
                    && fp_done == 1) {
                break;
            }
            continue;
        }

        pthread_mutex_lock(&ioq_mtx);
        if(DEBUG){
            printf("\n|||||||||||||IO LOOP||||||||||||||||\n");
            printf("Complete Size: %d\n", complete_queue->size);
            printf("IO Size: %d\n", io_queue->size);
            printf("CPU Size: %d\n", ready_queue->size);
            printf("Proc Count: %d\n", *proc_count);
            // printf("SemaphoreVal: %d\n", sem_value);
            // printf("MTX result: %d\n\n", mtx_result);
        }
        proc = popFirst(io_queue); // grab first process from io_queue
        pthread_mutex_unlock(&ioq_mtx);

        if (proc == NULL) {
            continue;
        }

        if(DEBUG){
            printf("\nProcess Popped (IO)\n");
            print_process(proc);
        }

        // set up struct for nanosleep
        struct timespec sleep_time = {0, (get_burst_time(proc)*1000000)};
        struct timespec remaining_time = {0,1};

        if(DEBUG) {printf("\nIO: Sleep proc %d for %d\n", proc->priority, get_burst_time(proc));}
        nanosleep(&sleep_time, &remaining_time);

        proc->index++;

        pthread_mutex_lock(&readyq_mtx);
        append(ready_queue, proc); // send back to cpu_scheduler
        pthread_mutex_unlock(&readyq_mtx);

    }

    if(DEBUG) {printf("Exiting IO Thread\n"); }
    sem_post(&thread_access);
    return NULL;
}
