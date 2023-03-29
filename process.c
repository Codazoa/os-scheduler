#include "process.h"

// create a new process with the given priority and burst time array
Process *create_proc(int priority, int *burst_times) {
    Process *new_proc = malloc(sizeof(Process));
    new_proc->priority = priority; 
    new_proc->burst_times = burst_times;
    new_proc->index = 0;
    return new_proc;
}

// return the current burst time pointed at by index
int get_burst_time(Process *proc) {
    proc->index = proc->index + 1;
    return proc->burst_times[(proc->index) - 1];
}