#include "process.h"

// create a new process with the given priority and burst time array
Process *create_proc(int priority, int *burst_times, int burst_count, struct timeval start_time) {
    Process *new_proc = malloc(sizeof(Process));
    new_proc->priority = priority;
    for (int i = 0; i < burst_count; i++) {
        new_proc->burst_times[i] = burst_times[i];
    } 
    new_proc->index = 0;
    new_proc->burst_count = burst_count;
    new_proc->start = start_time;

    memset(&new_proc->wait, 0, sizeof(struct timeval));
    memset(&new_proc->end, 0, sizeof(struct timeval));
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
    printf("StartTime: %ld.%ld\n", 
        proc->start.tv_sec,
        proc->start.tv_usec);
    printf("WaitTime: %f\n", getTimeInMs(proc->wait));
    printf("EndTime: %f\n", getTimeInMs(proc->end));
}

void calcWaitTime(Process *proc) {
    struct timeval current_time;
    gettimeofday(&current_time, NULL);

    // get difference between entered_ready and current time
    struct timeval time_diff = timeval_diff(proc->entered_ready, current_time);

    // update process wait time with new wait time
    proc->wait = timeval_add(proc->wait, time_diff);
}

struct timeval timeval_diff(struct timeval start, struct timeval end) {
    struct timeval diff;
    diff.tv_sec = end.tv_sec - start.tv_sec;
    diff.tv_usec = end.tv_usec - start.tv_usec;

    if (diff.tv_usec < 0) {
        diff.tv_sec -= 1;
        diff.tv_usec += 1000000;
    }

    return diff;
}

struct timeval timeval_add(struct timeval a, struct timeval b) {
    struct timeval result;
    result.tv_sec = a.tv_sec + b.tv_sec;
    result.tv_usec = a.tv_usec + b.tv_usec;

    if (result.tv_usec >= 1000000) {
        result.tv_sec += 1;
        result.tv_usec -= 1000000;
    }

    return result;
}

float getTimeInMs(struct timeval a){
	return ((float)a.tv_sec * 1000 + (float)a.tv_usec / 1000);
}