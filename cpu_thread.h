#ifndef CPU_THREAD_H
#define CPU_THREAD_H

#include <stdio.h>
#include "double_linked_list.h"

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

void *start_scheduler(void *arg);

#endif