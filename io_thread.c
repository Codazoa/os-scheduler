#include "io_thread.h"
        
// when we get created
// grab our shared IO queue
// create io queue


// when a process enters our pipe
    // append it to the io queue 

// while things are in our queue
    // pull first process from queue
    // sleep for it's time
    // append to shared ready_queue
    // send back through pipe

// when we recieve EOF
    // exit our thread


//Start running the IO thread
void *startIO(void *arg) {
    bool eofFlag = true;                    //Flag used to escape the loop once we read EOF
    Process *proc;                          //Variable used to hold current process

    // create the ready queue
    DoublyLinkedList *Queue = create_list();

    while(eofFlag){

        //Read from our pipe
        int n1 = read(parse_cpu_pipe[2], proc, sizeof(Process));
        if(n1 > 0){                         //New process, add to queue
            append(Queue, proc);
        }

        if(!isEmpty(Queue)){
            proc = popFirst(Queue);
            sleep(get_next_burst(proc));
            //TODO: <<<<< SEND proc BACK TO CPU SCHEDULER QUEUE
        }

        if(n1 == 0){                        //We have reached end of file           
            eofFlag = false;
        }

    }

}
