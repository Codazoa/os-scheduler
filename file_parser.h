#ifndef FILE_PARSER_H
#define FILE_PARSER_H

#include <string.h>
#include <stdio.h>
#include <unistd.h>

// One thread is responsible for reading the input file. This corresponds to 
// user activity causing new processes to come into existence. Any time this 
// thread sees a proc line, it should create a new ‘process’ and put it in 
// the ready queue. The ‘process’ is not a real process, it just represents 
// the characteristics of the process to simulate. When the thread encounters
// a sleep X it should go to sleep for X milliseconds, before going back to
// reading the file. This thread quits once it reads stop.

void *parse_file(void *arg);

#endif