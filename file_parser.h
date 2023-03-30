#ifndef FILE_PARSER_H
#define FILE_PARSER_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#include "process.h"
#include "double_linked_list.h"

#define DEBUG 1


// One thread is responsible for reading the input file. This corresponds to 
// user activity causing new processes to come into existence. Any time this 
// thread sees a proc line, it should create a new ‘process’ and put it in 
// the ready queue. The ‘process’ is not a real process, it just represents 
// the characteristics of the process to simulate. When the thread encounters
// a sleep X it should go to sleep for X milliseconds, before going back to
// reading the file. This thread quits once it reads stop.

extern sem_t thread_access;

extern pthread_mutex_t readyq_mtx;
extern pthread_mutex_t proc_count_mtx;

typedef struct {
    FILE *fp; // file pointer for our input file (already valid)
    DoublyLinkedList *ready_queue; // pointer to ready_queue shared memory
    int *proc_count; // share the process count
} Parser_args_t;

void *parse_file(void *arg);

#endif