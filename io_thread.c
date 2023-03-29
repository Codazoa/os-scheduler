#include "io_thread.h"

// when we get created
// grab our shared IO queue

// while things are in our queue
    // pull first process from queue
    // sleep for it's time
    // append to shared ready_queue
