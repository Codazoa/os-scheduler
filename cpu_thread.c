#include "cpu_thread.h"


// when we get created
// create ready_queue

// read from both pipes

// if either pipe has a process
    // append it to the ready queue



// start the cpu scheduler
void *start_scheduler(void *arg) {
    // check the algorithm passed
    int algo = *(int*)arg;

    printf("Algorithm is %d\n", algo); //DEBUG

    // create the ready queue
    DoublyLinkedList *ready_queue = create_list();

    return NULL;
}