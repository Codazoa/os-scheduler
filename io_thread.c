#include "io_thread.h"

// when we get created
// create io queue 

// when a process enters our pipe
    // append it to the io queue 

// while things are in our queue
    // pull first process from queue
    // sleep for it's time
    // send back through pipe

// when we recieve EOF
    // exit our thread