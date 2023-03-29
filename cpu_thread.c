#include "cpu_thread.h"


// when we get created
// create ready_queue

// read from both pipes

// if either pipe has a process
    // append it to the ready queue

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

    printf("Algorithm is %d\n", algo); //DEBUG
    printf("Quantum is %d\n", quantum);

    // create the ready queue
    DoublyLinkedList *ready_queue = create_list();

    return NULL;
}