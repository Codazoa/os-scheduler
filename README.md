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
- 

> How you switch between scheduling algorithms
- The requested scheduling algorithm is checked for in main before any threads are created. Each 

> How you generate data for the required measurements
- 

> The purposes of any threads beyond the three required
- We are only using the 3 threads that are required (file_parser, cpu_thread, io_thread)