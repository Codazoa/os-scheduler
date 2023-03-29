#ifndef DOUBLE_LINKED_LIST
#define DOUBLE_LINKED_LIST

#include <stdbool.h> 

#include "process.h"

// struct to hold node data
typedef struct Node{
    Process *proc;
    struct Node *prev;
    struct Node *next;
} Node;

// struct to hold linked list data
typedef struct {
    Node *head;
    Node *tail;
    int size;
} DoublyLinkedList;

// create an empty linked list
DoublyLinkedList *create_list();

// append a node with data in it
void append(DoublyLinkedList *list, Process *new_proc);

//Pop off first proc of list
Process *popFirst(DoublyLinkedList *list);

//Return the highest priority proc of the linked list
Process *popHighP(DoublyLinkedList *list);

//Return the lowest total time remaining proc of the linked list
Process *popLeastTimeTotal(DoublyLinkedList *list);

//Return the lowest next time remaining proc of the linked list
Process *popLeastTimeIndv(DoublyLinkedList *list);

//Return true if the list is empty
bool isEmpty(DoublyLinkedList *list);

#endif