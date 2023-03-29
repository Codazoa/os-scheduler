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

    Process *proc; // variable used to hold current process

    while(1){

        if(!isEmpty(io_queue)){
            proc = popFirst(io_queue); // grab first process from io_queue
            sleep(get_next_burst(proc)); // sleep for designated burst time
            append(ready_queue, proc); // send back to cpu_scheduler
        }

        // check if we got the signal to end and break
    }

    return NULL;
}
