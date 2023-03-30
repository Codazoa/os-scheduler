#include "file_parser.h"

// Sample File Format
// proc PRIORITY NUM_OF_BURSTS CPU_BURST IO_BURST CPU_BURST . . .
// sleep TIME_TO_SLEEP_ms
// stop
// 
// proc: a process to simulate, always ends on a CPU_BURST
// PRIORITY: 1-10, 1-lowest 10-highest 5-average
// NUM_OF_BURSTS: how many CPU and IO bursts will follow
// CPU_BURST: CPU burst time in ms
// IO_BURST: IO burst time in ms
// TIME_TO_SLEEP: how many ms for the parser thread to sleep


// take a file, parse it
// spawn new thread for each CPU and IO burst
void *parse_file(void *arg){
    // FILE *fp = (FILE *)arg;
    Parser_args_t *parser_args = (Parser_args_t*) arg;
    FILE *fp = parser_args->fp;
    DoublyLinkedList *ready_queue = parser_args->ready_queue;

    if (DEBUG) {printf("Parsing File\n");}

    char line[100], *word;

    while (fgets(line, sizeof(line), fp) != NULL) {
        word = strtok(line, " \t\n");
        if (strcmp(word, "proc") == 0) {
            // create process structure based on given values
            // Convert second word to integer for PRIORITY
            word = strtok(NULL, " \t\n");
            int priority = atoi(word);
            if (priority == 0 && word[0] != 0) {
                fprintf(stderr, "Error: conversion error, given priority is not an integer\n");
                exit(1);
            }

            // Convert third word to integer for NUM OF BURSTS
            word = strtok(NULL, " \t\n");
            int burst_count = atoi(word);
            if (burst_count == 0 && word[0] != 0) {
                fprintf(stderr, "Error: conversion error, given burst_count is not an integer\n");
                exit(1);
            }

            // Convert the rest of the line to integers for individual bursts
            int bursts[burst_count];
            word = strtok(NULL, " \t\n");
            for(int i = 0; i < burst_count; i++) {
                int burst = atoi(word);
                if (burst == 0 && word[0] != 0) {
                    fprintf(stderr, "Error: conversion error, given burst is not an integer\n");
                    exit(1);
                } else {
                    bursts[i] = burst;
                }
                // get next word before moving on
                word = strtok(NULL, " \t\n");
            }

            // get start time of new process
            struct timeval start_time;
            gettimeofday(&start_time, NULL);

            // create a new process with priority and burst times
            Process *new_process = create_proc(priority, bursts, burst_count, start_time);

            if(DEBUG){ 
                printf("\nAfter process Creation\n");
                print_process(new_process);
            }


            pthread_mutex_lock(&readyq_mtx);
            append(ready_queue, new_process);
            pthread_mutex_unlock(&readyq_mtx);
            

        } else if (strcmp(word, "sleep") == 0) {
            // sleep for the given number of milliseconds
            word = strtok(NULL, " \t\n");

            int sleep_time = atoi(word);
            if (sleep_time == 0 && word[0] != 0) {
                fprintf(stderr, "Error: conversion error, given sleep time is not an integer\n");
                exit(1);
            } else {
                printf("DEBUG: sleeping for %d ms\n", sleep_time);
                sleep(sleep_time/1000);
            }

        } else if (strcmp(word, "stop") == 0) {
            //stop the program
            return NULL;
        } else {
            fprintf(stderr, "Error: invalid input. please check that the file is correct\n");
            exit(1);
        }
    }

    return NULL;
}