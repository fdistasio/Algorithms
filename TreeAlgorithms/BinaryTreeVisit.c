//Binary Tree Visits
#include <stdlib.h>

typedef struct n{
    int data;
    struct n * r;
    struct n * l;
}node;

//O(n)
void binaryTreePreorderVisit(node* root){
    if(root != NULL){
        printf("%d ",root->data);
        binaryTreePreorderVisit(root->l);
        binaryTreePreorderVisit(root->r);
    }
}

//O(n)
void binaryTreeInorderVisit(node* root){
    if(root != NULL){
        binaryTreeInorderVisit(root->l);
        printf("%d ",root->data);
        binaryTreeInorderVisit(root->r);
    }
}

//O(n)
void binaryTreePostorderVisit(node* root){
    if(root != NULL){
        binaryTreePostorderVisit(root->l);
        binaryTreePostorderVisit(root->r);
        printf("%d ",root->data);
    }
}
