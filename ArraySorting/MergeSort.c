//Merge Sort O(nlogn)

#include <limits.h>

void merge(int A[], int left, int mid, int right){
    int n1 = mid - left + 1;
    int n2 = right - mid;
    int L[n1+1], R[n2+1];

    for(int i = 0 ; i < n1 ; i++)
        L[i] = A[left+i];
    for(int i = 0 ; i < n2 ; i++)
        R[i] = A[mid+i+1];
        
    L[n1] = INT_MAX;
    R[n2] = INT_MAX;

    int i = 0, j = 0;
    for(int k = left ; k <= right ; k++){
        if(L[i] <= R[j]){
            A[k] = L[i];
            i++;
        }
        else{
            A[k] = R[j];
            j++;
        }
    }

}

void mergeSort(int A[], int left , int right){
    if(left < right){
        int mid = (left + right)/2;
        mergeSort(A,left,mid);
        mergeSort(A,mid+1,right);
        merge(A,left,mid,right);
    }
}