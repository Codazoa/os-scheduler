#ifndef PROCESS_H
#define PROCESS_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/time.h>

typedef struct Process {
    int priority; // process's priority level
    int burst_times[100]; // burst time array
    int index; // where we are in the burst times array
    int burst_count; // count of bursts 
    struct timeval start;
    struct timeval wait;
    struct timeval end;
    struct timeval entered_ready; // time we entered the ready queue
} Process;

// create a new process with the given priority and burst time array
Process *create_proc(int priority, int *burst_times, int burst_count, struct timeval start_time);

// return the current burst time pointed at by index
int get_burst_time(Process *proc);

// return next burst time
int get_next_burst(Process *proc);

void print_process(Process *proc);

//Calculate and update the wait time for the given process
void calcWaitTime(Process *proc);

struct timeval timeval_diff(struct timeval start, struct timeval end);

struct timeval timeval_add(struct timeval a, struct timeval b);

float getTimeInMs(struct timeval a);

#endif