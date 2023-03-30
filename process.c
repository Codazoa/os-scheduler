#include "process.h"

// create a new process with the given priority and burst time array
Process *create_proc(int priority, int *burst_times, int burst_count) {
    Process *new_proc = malloc(sizeof(Process));
    new_proc->priority = priority;
    for (int i = 0; i < burst_count; i++) {
        new_proc->burst_times[i] = burst_times[i];
    } 
    new_proc->index = 0;
    new_proc->burst_count = burst_count;
    memset(new_proc->start_wait_end_time, 0, sizeof(new_proc->start_wait_end_time));
    return new_proc;
}

// return the current burst time pointed at by index
int get_burst_time(Process *proc) {
    return proc->burst_times[proc->index];
}

// return next burst time
int get_next_burst(Process *proc) {
    proc->index++;
    return proc->burst_times[proc->index];
}

void print_process(Process *proc){
    printf("Priority: %d\n", proc->priority);
    printf("BurstNum: %d\n", proc->burst_count);
    for(int i = 0; i < proc->burst_count; i++){
        printf("%d ", proc->burst_times[i]);
    }
    printf("\n");
    printf("StartTime: %ld\nWaitTime: %ld\nEndTime: %ld\n", 
        proc->start_wait_end_time[0],proc->start_wait_end_time[1],proc->start_wait_end_time[2]);
}

void calcWaitTime(Process *proc) {
    struct timeval current_time;
    gettimeofday(&current_time, NULL);

    // convert time to ms
    long int time_diff = (current_time.tv_sec - proc->entered_ready.tv_sec) * 1000000 +
                         (current_time.tv_usec - proc->entered_ready.tv_usec);

    // update process wait time with new wait time
    proc->start_wait_end_time[1] += time_diff / 1000;
}