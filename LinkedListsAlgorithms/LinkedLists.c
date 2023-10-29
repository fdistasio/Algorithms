//Linked Lists functions
#include <stdlib.h>

typedef struct n{
    int data;
    struct n * next;
}Node;

//O(1)
void headInsert(Node** head,int data){
    Node *newNode = (Node*)malloc(sizeof(Node*));
    if(newNode == NULL) exit(1);
    newNode->data = data;
    newNode->next = NULL;

    newNode->next = *head;
    *head = newNode;
}

//O(n)
void sortedInsert(Node** head,int data){
    Node *newNode = (Node*)malloc(sizeof(Node*));
    if(newNode == NULL) exit(1);
    newNode->data = data;
    newNode->next = NULL;

    Node *prevPtr = NULL;
    Node *currPtr = *head;

    while(currPtr != NULL && data > currPtr->data){
        prevPtr = currPtr;
        currPtr = currPtr->next;
    }

    if(prevPtr == NULL){
        newNode->next = *head;
        *head = newNode;
    }
    else{
        prevPtr->next = newNode;
        newNode->next = currPtr;
    }
}

//O(n)
void deleteData(Node** head, int data){
    if(*head == NULL) return;
    
    Node *prevPtr = NULL;
    Node *currPtr = *head;

    while(currPtr != NULL){
        if(currPtr->data == data){
            Node *tempPtr = currPtr;
            
            if(prevPtr != NULL){
                prevPtr->next = currPtr->next;
            }
            else{
                *head = currPtr->next;
            }
            
            currPtr = currPtr->next;
            free(tempPtr);
        }
        else{
            prevPtr = currPtr;
            currPtr = currPtr->next;
        }
    }
    
}

//O(n)
void printL(Node* head){
    Node *currPtr = head;
    while(currPtr != NULL){
        printf("%d ",currPtr->data);
        currPtr = currPtr->next;
    }
}

//O(n)
void sortedInsertRec(Node** head, int data){
    if(*head == NULL){
        Node *newNode = (Node*)malloc(sizeof(Node*));
        if(newNode == NULL) exit(1);
        newNode->data = data;
        newNode->next = NULL;
        *head = newNode;
    }
    else if((*head)->data > data){
        Node *newNode = (Node*)malloc(sizeof(Node*));
        if(newNode == NULL) exit(1);
        newNode->data = data;
        newNode->next = *head;
        *head = newNode;
    }
    else
        sortedInsertRec(&((*head)->next),data);

}

//O(n)
void deleteDataRec(Node** head, int data){
    if(*head == NULL) return;
    if((*head)->data == data){
        Node *tempPtr = *head;
        *head = tempPtr->next;
        free(tempPtr);
        deleteDataRec(&(*head),data);
    }
    else
        deleteDataRec(&(*head)->next,data);
}

//O(n)
void printLRec(Node* head){
    if(head == NULL) return;
    printf("%d",head->data);
    printLRec(head->next);
}
