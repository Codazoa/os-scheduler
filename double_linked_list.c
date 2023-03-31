#include "double_linked_list.h"

DoublyLinkedList *create_list() {
    DoublyLinkedList *list = malloc(sizeof(DoublyLinkedList));
    list->head = NULL;
    list->tail = NULL;
    return list;
}

void append(DoublyLinkedList *list, Process *new_proc) {
    //create the new node
    Node *new_node = malloc(sizeof(Node));
    new_node->proc = new_proc; 
    new_node->prev = list->tail;
    new_node->next = NULL;
    gettimeofday(&(new_proc->entered_ready), NULL); // set the time process entered the queue
    if(DEBUG) {printf("Entered ready queue at: %ld.%ld", new_proc->entered_ready.tv_sec, new_proc->entered_ready.tv_usec);}

    // list has a tail
    if (list->tail != NULL) {
        list->tail->next = new_node;
    }

    // set tail to new end
    list->tail = new_node;

    // if list has no head, 
    if (list->head == NULL) {
        list->head = new_node;
    }  
    // increment list size
    list->size++;
}

//isEmpty
//isEmpty returns true if the DLL does not have a head, otherwise it returns false
bool isEmpty(DoublyLinkedList *list){
    if(list->head == NULL){
        return true;
    }
    return false;
}

//getSize
//getSize returns the size of the DLL
int getSize(DoublyLinkedList *list){
    return list->size;
}

//popFirst
//This function will return the head process of the doubly linked list
Process *popFirst(DoublyLinkedList *list){
    Node *oldHead = list->head;                 //Store the current head  
    Process *storage;                           //Variable to hold the process while the node is freed

    if (oldHead == NULL){                       //Make sure list isn't empty
        return NULL;
    }
    storage = oldHead->proc;                    //Store the head proc in storage

    if(list->head->next != NULL){               //Is there a next node
        list->head = list->head->next;          //Make the head's next the new head
        list->head->prev = NULL;                //Clear the old head from the current head's link
    }else{
        list->head = NULL;
        list->tail = NULL;
    }
    free(oldHead);                              //Free the memory allocated to oldHead
    list->size--;                               //Decrement list size
    if(storage->index % 2 == 0) {calcWaitTime(storage);}
    return storage;                             //Return the process
}

//popHighP
//This function will return the highest priority process
Process *popHighP(DoublyLinkedList *list){
    int priorityCounter = 0;                    //Variable to keep track of the highest priority
    Node *curNode = list->head;                 //Store the head in the curNode variable;                             
    Process *storage;                            //Variable to hold the process while the node is freed

    if(curNode == NULL){                        //Make sure the list isn't empty
        return NULL;
    }

    //Go through the list and find the highest priority
    while(curNode != NULL){

        //Overwrite the priorityCounter if the current node has a higher priority
        if(curNode->proc->priority > priorityCounter){
            if(curNode->proc->priority == 10){  //Check if this is the highest priority
                storage = curNode->proc;        //Move the proc to storage

                //Repair the list for when we remove the node
                if(curNode->next != NULL && curNode->prev != NULL){ //If we have a node before and after us
                    curNode->next->prev = curNode->prev;            //Splice us out of the list
                    curNode->prev->next = curNode->next;
                }else if(curNode->next != NULL){                    //We only have a next node (We are the head)
                    curNode->next->prev = NULL;                     //Assign the next node to be the head
                    list->head = curNode->next;
                }else if(curNode->prev != NULL){                    //We only have a previous node (We are the tail)
                    curNode->prev->next = NULL;                     //Assign the prev node to be the tail
                    list->tail = curNode->prev;
                }else{                                              //We do not have a next or previous (We are head and tail)
                    list->head = NULL;
                    list->tail = NULL;
                }

                free(curNode);                  //Free the memory used by node
                list->size--;                   //Decrement list size
                calcWaitTime(storage);
                return storage;                 //If it is, just return it
            }
            priorityCounter = curNode->proc->priority;
        }
        curNode = curNode->next;                //Set curNode to the next node
    }

    curNode = list->head;                       //Set curNode back to the list's head

    //Now go back through the lsit until we find the highest priority
    while(curNode != NULL){
        
        //Check to see if curNode matches our highest priority
        if(curNode->proc->priority == priorityCounter){
            storage = curNode->proc;            //Save the proc in storage

            //Repair the list for when we remove the node
            if(curNode->next != NULL && curNode->prev != NULL){ //If we have a node before and after us
                curNode->next->prev = curNode->prev;            //Splice us out of the list
                curNode->prev->next = curNode->next;
            }else if(curNode->next != NULL){                    //We only have a next node (We are the head)
                curNode->next->prev = NULL;                     //Assign the next node to be the head
                list->head = curNode->next;
            }else if(curNode->prev != NULL){                    //We only have a previous node (We are the tail)
                curNode->prev->next = NULL;                     //Assign the prev node to be the tail
                list->tail = curNode->prev;
            }else{                                              //We do not have a next or previous (We are head and tail)
                list->head = NULL;
                list->tail = NULL;
            }

            free(curNode);                      //Free the memory allocated to curNode
            list->size--;                       //Decrement list size
            calcWaitTime(storage);
            return storage;                     //Return this proc
        }
        curNode = curNode->next;                //Set curNode to the next node
    }
    
    
    return NULL;                                //Backup return in case something messes up
}

//popLeastTimeTotal
//This function will return the proc with the least total time remaining
Process *popLeastTimeTotal(DoublyLinkedList *list){
    int lowestTime = 9999;                      //Variable to keep track of the lowest time
    int timer = 0;                              //Variable to keep track of the current total time p0er proc
    Node *curNode = list->head;                 //Store the head in the curNode variable;                             
    Process *storage;                            //Variable to hold the process while the node is freed

    if(curNode == NULL){                        //Make sure the list isn't empty
        return NULL;
    }

    while(curNode != NULL){

        timer = 0;                              //Reset timer back to zero

        //Add all of the burst times together from the current node
        for(int i = curNode->proc->index; i < curNode->proc->burst_count; i++){
            timer = timer + curNode->proc->burst_times[i];
        }
        
        if(timer < lowestTime){                 //Check if the total value is less than the lowest value
            lowestTime = timer;                 //Set lowestTime to the total time from the process
        }

        curNode = curNode->next;                //set curNode to the next node
    }

    curNode = list->head;                       //Set curNode back to the head

    while(curNode != NULL){

        timer = 0;                              //Reset timer back to zero

        //Add all of the burst times together from the current node
        for(int i = curNode->proc->index; i < curNode->proc->burst_count; i++){
            timer = timer + curNode->proc->burst_times[i];
        }

        if(timer == lowestTime){                //If this has the lowest time remaining return it
            storage = curNode->proc;            //Save the proc in storage

            //Repair the list for when we remove the node
            if(curNode->next != NULL && curNode->prev != NULL){ //If we have a node before and after us
                curNode->next->prev = curNode->prev;            //Splice us out of the list
                curNode->prev->next = curNode->next;
            }else if(curNode->next != NULL){                    //We only have a next node (We are the head)
                curNode->next->prev = NULL;                     //Assign the next node to be the head
                list->head = curNode->next;
            }else if(curNode->prev != NULL){                    //We only have a previous node (We are the tail)
                curNode->prev->next = NULL;                     //Assign the prev node to be the tail
                list->tail = curNode->prev;
            }else{                                              //We do not have a next or previous (We are head and tail)
                list->head = NULL;
                list->tail = NULL;
            }
            
            free(curNode);                      //Free the memory allocated to curNode
            list->size--;                       //Decrement list size
            calcWaitTime(storage);
            return storage;
        }
        curNode = curNode->next;                //set curNode to the next node
    }
    return NULL;                                //Backup return if something bad happens
}

//popLeastTimeIndv
//This function will return the proc with the least next time remaining
Process *popLeastTimeIndv(DoublyLinkedList *list){
    int lowestTime = 9999;                      //Variable to keep track of the lowest time
    Node *curNode = list->head;                 //Store the head in the curNode variable;                             
    Process *storage;                            //Variable to hold the process while the node is freed

    if(curNode == NULL){                        //Make sure the list isn't empty
        return NULL;
    }

    while(curNode != NULL){

        //Check if the next burst time is lower than the current record
        if(curNode->proc->burst_times[curNode->proc->index] < lowestTime){
            lowestTime = curNode->proc->burst_times[curNode->proc->index];
        }
        curNode = curNode->next;                //set curNode to the next node
    }

    curNode = list->head;                       //Set curNode back to the head

    while(curNode != NULL){

        //Check if the next burst time is lower than the current record
        if(curNode->proc->burst_times[curNode->proc->index] == lowestTime){
            storage = curNode->proc;            //Save the proc in storage

            //Repair the list for when we remove the node
            if(curNode->next != NULL && curNode->prev != NULL){ //If we have a node before and after us
                curNode->next->prev = curNode->prev;            //Splice us out of the list
                curNode->prev->next = curNode->next;
            }else if(curNode->next != NULL){                    //We only have a next node (We are the head)
                curNode->next->prev = NULL;                     //Assign the next node to be the head
                list->head = curNode->next;
            }else if(curNode->prev != NULL){                    //We only have a previous node (We are the tail)
                curNode->prev->next = NULL;                     //Assign the prev node to be the tail
                list->tail = curNode->prev;
            }else{                                              //We do not have a next or previous (We are head and tail)
                list->head = NULL;
                list->tail = NULL;
            }

            free(curNode);                      //Free the memory allocated to curNode
            list->size--;                       //Decrement list size
            calcWaitTime(storage);
            return storage;                     //Set lowestTime to the total time from the process
        }
        curNode = curNode->next;                //set curNode to the next node
    }
    return NULL;                                //Backup return if something bad happens
}