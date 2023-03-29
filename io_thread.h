#ifndef IO_THREAD_H
#define IO_THREAD_H

#include "double_linked_list.h"

// This thread simulates the I/O subsystem. If there is anything waiting in
// the I/O queue, select a process in FIFO order. Sleep for the given I/O 
// burst time (to simulate waiting on an I/O device) and then put the process
// back in the ready queue. Repeat until there are no more jobs.

extern int cpu_io_pipe[2];

typedef struct {
    DoublyLinkedList *io_queue;
} io_args_t;

#endif