#ifndef IO_THREAD_H
#define IO_THREAD_H

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#include "double_linked_list.h"

extern sem_t thread_access;

// This thread simulates the I/O subsystem. If there is anything waiting in
// the I/O queue, select a process in FIFO order. Sleep for the given I/O 
// burst time (to simulate waiting on an I/O device) and then put the process
// back in the ready queue. Repeat until there are no more jobs.

extern pthread_mutex_t readyq_mtx;
extern pthread_mutex_t ioq_mtx;

typedef struct {
    DoublyLinkedList *io_queue;
    DoublyLinkedList *ready_queue;
} io_args_t;

void *startIO(void *arg); // start the io thread

#endif