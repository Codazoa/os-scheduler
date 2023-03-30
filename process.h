#ifndef PROCESS_H
#define PROCESS_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct Process {
    int priority; // process's priority level
    int burst_times[100]; // burst time array
    int index; // where we are in the burst times array
    int burst_count; // count of bursts 
    time_t start_wait_end_time[3]; // to hold a list of start, wait, end times
    time_t entered_ready; // time we entered the ready queue
} Process;

// create a new process with the given priority and burst time array
Process *create_proc(int priority, int *burst_times, int burst_count);

// return the current burst time pointed at by index
int get_burst_time(Process *proc);

// return next burst time
int get_next_burst(Process *proc);

void print_process(Process *proc);

#endif