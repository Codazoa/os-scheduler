#include "cpu_thread.h"

// start the cpu scheduler
void *start_scheduler(void *arg) {
    // check the algorithm passed
    int algo = *(int*)arg;

    printf("Algorithm is %d\n", algo); //DEBUG

    // create the ready queue
    DoublyLinkedList *ready_queue = create_list();

    return NULL;
}