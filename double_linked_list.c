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



