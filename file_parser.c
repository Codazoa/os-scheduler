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