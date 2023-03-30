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

    // pthread_mutex_lock(&thread_running_mtx);
    // pthread_cond_signal(&thread_running_cond);
    // pthread_mutex_unlock(&thread_running_mtx);

    // wait for main to allow us to continue
    sem_wait(&thread_access);

    while(1){

        // int mtx_result = pthread_mutex_trylock(&thread_running_mtx);

        if(isEmpty(io_queue)){
            int sem_value;
            sem_getvalue(&thread_access, &sem_value);
            
            int mtx_result = pthread_mutex_trylock(&thread_running_mtx);

            if(DEBUG){
                printf("\n|||||||||||||IO EXIT||||||||||||||||\n");
                printf("Complete Size: %d\n", complete_queue->size);
                printf("IO Size: %d\n", io_queue->size);
                printf("CPU Size: %d\n", ready_queue->size);
                printf("Proc Count: %d\n", *proc_count);
                printf("SemaphoreVal: %d\n", sem_value);
                printf("MTX result: %d\n\n", mtx_result);
            }
            


            // if all jobs are in complete queue and
            // one other thread has finished and
            // the mutex was able to be locked
            if (complete_queue->size == *proc_count 
                    && sem_value == 1 
                    && mtx_result == 0 ) {
                pthread_mutex_unlock(&thread_running_mtx);
                break;
            }
            pthread_mutex_unlock(&thread_running_mtx);
            continue;
        }


        pthread_mutex_lock(&ioq_mtx);
        proc = popFirst(io_queue); // grab first process from io_queue
        pthread_mutex_unlock(&ioq_mtx);

        printf("\nProcess Popped (IO)\n");
        print_process(proc);

        printf("\nIO: Sleep proc %d for %d\n", proc->priority, get_burst_time(proc));
        sleep(get_burst_time(proc)/1000); // sleep for designated burst time
        proc->index++;

        pthread_mutex_lock(&readyq_mtx);
        append(ready_queue, proc); // send back to cpu_scheduler
        pthread_mutex_unlock(&readyq_mtx);

    }

    if(DEBUG) {printf("Exiting IO Thread\n"); }
    sem_post(&thread_access);
    return NULL;
}
