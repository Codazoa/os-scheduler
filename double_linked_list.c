#include "double_linked_list.h"
#include <stdlib.h>

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
}

//popFirst
//This function will remove the head of the doubly linked list
Process popFirst(DoublyLinkgedList *list){

    
    if (list->head == NULL){            //Make sure list isn't empty
        return NULL;
    }
    Node *oldHead = list->head;         //Store the current head         

    if(list->head->next != NULL){       //Is there a next node
        list->head = list->head->next;  //Make the head's next the new head
        list->head->prev = NULL;        //Clear the old head from the current head's link
    }

    return oldHead->proc;               //Return the process

}



