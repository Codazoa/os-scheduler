#ifndef IO_THREAD_H
#define IO_THREAD_H

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#include "double_linked_list.h"
#include "options.h"


// This thread simulates the I/O subsystem. If there is anything waiting in
// the I/O queue, select a process in FIFO order. Sleep for the given I/O 
// burst time (to simulate waiting on an I/O device) and then put the process
// back in the ready queue. Repeat until there are no more jobs.

extern sem_t thread_access;

extern pthread_mutex_t readyq_mtx;
extern pthread_mutex_t ioq_mtx;
extern pthread_mutex_t proc_count_mtx;

extern pthread_mutex_t thread_running_mtx;
extern pthread_cond_t thread_running_cond;
extern int fp_done;

typedef struct {
    DoublyLinkedList *io_queue; // pointer to shared io_queue
    DoublyLinkedList *ready_queue; // pointer to shared ready_queue
    DoublyLinkedList *complete_queue;
    int *proc_count; // share the process count
} io_args_t;

void *startIO(void *arg); // start the io thread

#endif