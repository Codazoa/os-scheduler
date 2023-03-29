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

    
    ssize_t n1, n2;
    // read from both pipes
    while (1) {
        Process *new_parse_proc = NULL;
        Process *new_io_proc = NULL;
        n1 = read(parse_cpu_pipe[0], new_parse_proc, sizeof(Process));
        // n2 = read(cpu_io_pipe[0], new_io_proc, sizeof(Process));

        if (n1 == 0 && n2 == 0) {
            // both pipes have reached EOF
            break;
        }

        if (n1 > 0) {
            // got a process from parser
            // add new_parse_proc to ready_queue
        }

        if (n2 > 0) {
            // got a process from io thread
            // add new_io_proc to ready_queue
        }

        // check if there is anything in the ready queue
        if (ready_queue->head == NULL){
            continue;
        }
    }

    return NULL;
}