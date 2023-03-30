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

    // wait for main to allow us to continue
    sem_wait(&thread_access);

    while(1){

        if(!isEmpty(io_queue)){
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
        } else {
            int sem_value;
            sem_getvalue(&thread_access, &sem_value);
            if (complete_queue->size == *proc_count && sem_value > 0) {
                break;
            }
        }


    }
    sem_post(&thread_access);
    return NULL;
}
