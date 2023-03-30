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

    if(DEBUG){
        printf("Algorithm is %d\n", algo); //DEBUG
        printf("Quantum is %d\n", quantum);
    }


    //debug
    // sleep(5);
    // pthread_mutex_lock(&readyq_mtx);
    // while(!isEmpty(ready_queue)){
    //     // Process *data = popFirst(ready_queue);
    //     Process *data = popFirst(ready_queue);
    //     print_process(data);
    // }
    // pthread_mutex_unlock(&readyq_mtx);
    
    while (1) {
        // check if there is anything in the ready queue
        if (isEmpty(ready_queue)){
            printf("ready Q is empty\n");
            continue;
        }

        Process *next_proc;


        switch (algo) {
            case 1: //First Come First Serve
                // pop first off ready queue
                pthread_mutex_lock(&readyq_mtx);
                next_proc = popFirst(ready_queue);
                pthread_mutex_unlock(&readyq_mtx);

                // printf("New Process\nPriority: %d\n", next_proc->priority);
                // // sleep for its time
                printf("Sleep for %d\n", get_next_burst(next_proc));
                sleep(get_next_burst(next_proc));

                // push it onto the io queue 
                pthread_mutex_lock(&ioq_mtx);
                append(io_queue, next_proc);
                pthread_mutex_unlock(&ioq_mtx);
                break;
            case 2: //Shortest Job First
                break;
            case 3: //Priority Scheduling
                break; 
            case 4: //Round Robin
                break;
        }
    }

    return NULL;
}