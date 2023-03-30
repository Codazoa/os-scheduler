#ifndef CPU_THREAD_H
#define CPU_THREAD_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/select.h>

#include "double_linked_list.h"
#include "process.h"

#define DEBUG 1

// This thread is analogous to the kernel’s CPU scheduler. Its job is to check
// the ready queue for processes and, if there are any, pick one from the queue
// according to the designated scheduling algorithm and reserve CPU time for 
// the designated burst (or quantum) time. This means that the thread will sleep
// for an appropriate number of milliseconds. 

// Once the thread wakes back up it either:
// 1. Moves the process to the I/O queue
// 2. Returns it to the ready queue (if using Round Robin and the quantum expired)
// 3. Terminates the process if it completed its last CPU burst
// Then, schedule another process from the ready queue.

// extern int parse_cpu_pipe[2];
// extern int cpu_io_pipe[2];
extern pthread_mutex_t readyq_mtx;
extern pthread_mutex_t ioq_mtx;
extern pthread_mutex_t completeq_mtx;

typedef struct {
    int algo; // which algorithm we are using
    int quantum; // optionally what the quantum is (default=0)
    DoublyLinkedList *ready_queue; // pointer to ready_queue shared memory
    DoublyLinkedList *io_queue; // pointer to io_queue shared memory
    DoublyLinkedList *complete_queue; // complete_queue shared memory
} CPU_args_t;

void *start_scheduler(void *arg);

#endif