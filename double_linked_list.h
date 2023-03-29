#ifndef DOUBLE_LINKED_LIST
#define DOUBLE_LINKED_LIST

// struct to hold node data
typedef struct Node{
    void *data;
    struct Node *prev;
    struct Node *next;
} Node;

// struct to hold linked list data
typedef struct {
    Node *head;
    Node *tail;
} DoublyLinkedList;

// create an empty linked list
DoublyLinkedList *create_list();

// append a node with data in it
void append(DoublyLinkedList *list, void *data);


#endif