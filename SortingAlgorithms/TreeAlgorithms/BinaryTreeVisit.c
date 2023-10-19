//Preorder Visit O(n), Inorder Visit O(n), Postorder Visit (n)
#include <stdlib.h>

typedef struct n{
    int data;
    struct n * r;
    struct n * l;
}node;

void binaryTreePreorderVisit(node* root){
    if(root != NULL){
        printf("%d ",root->data);
        binaryTreePreorderVisit(root->l);
        binaryTreePreorderVisit(root->r);
    }
}

void binaryTreeInorderVisit(node* root){
    if(root != NULL){
        binaryTreeInorderVisit(root->l);
        printf("%d ",root->data);
        binaryTreeInorderVisit(root->r);
    }
}

void binaryTreePostorderVisit(node* root){
    if(root != NULL){
        binaryTreePostorderVisit(root->l);
        binaryTreePostorderVisit(root->r);
        printf("%d ",root->data);
    }
}
