# OS-Scheduler

### To run:
`$ ./scheduler -alg [FCFS,SJF,PR,RR] [-quantum [integer(ms)] -input [filename]`

Available Scheduling Algorithms:
- First Come First Serve
- Shortest Job First
- Priority Scheduling
- Round Robin

Quantum is only used for RR (Round Robin) scheduling algorithm

### Questions:
> How do you share data between parts of the program
- We create shared memory queues for the threads to communicate and mutexes to control access to the structures to prevent race conditions.

> Your approach to synchronization issues
- We use a semaphore to block threads until they have all been created. Mutexes protect access to shared memory such as the complete_queue, ready_queue, and io_queue. 
- The cpu_thread checkes for multiple conditions to be met before it can exit. The semaphore has to be equal to 2 meaning file_parser and io_thread have finished, the current ready_queue has to be empty, and the complete_queue size has to equal the number of processes we have run. The file parser sets a flag when it exits (fp_done) to tell the other processes it has finished.
- The io_thread essentially works the same as the cpu_thread for determining synchronization. 

> How you switch between scheduling algorithms
- The requested scheduling algorithm is checked for in main before any threads are created. Each 

> How you generate data for the required measurements
-   We used getTimeOfDay to get timestamps at critical moments(program start/end, ready_queue pop/append) which saved a time_val struct with
    seconds and micro seconds. We have several processor functions that use these time vals to get the turn around time of each process as well
    as wait time and total run time. We also converted the time_val times to milliseconds represented as integers.

> The purposes of any threads beyond the three required
- We are only using the 3 threads that are required (file_parser, cpu_thread, io_thread)