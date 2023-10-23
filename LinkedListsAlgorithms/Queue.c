//Queue Functions
#include <stdlib.h>

typedef struct n{
    int data;
    struct n * next;
}Node;

//O(1)
void enqueue(Node** head,Node** tail,int data){
    Node *newNode = (Node*)malloc(sizeof(Node*));
    if(newNode == NULL) exit(1);
    newNode->data = data;
    newNode->next = NULL;
    if(*head == NULL){
        *head = newNode;
        *tail = newNode;
    }
    else{
        (*tail)->next = newNode;
        *tail = newNode;
    }
}

//O(1)
void dequeue(Node** head,Node** tail){
    if(*head == NULL) return;
    Node* tempPtr = *head;
    *head = (*head)->next;
    if(*head == NULL){
        *tail = NULL;
    }
    free(tempPtr);
}