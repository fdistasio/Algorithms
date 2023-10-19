//Selection Sort O(n^2)

void selectionSort(int A[], int n){

    int i = 0, j=0, min=0;
    
    for(i = 0 ; i < n-1 ; i++){
        min = i;
        for(j = i+1 ; j < n ; j++){
            if(A[j] < A[min])
                min = j;
        }
        int temp = A[i];
        A[i] = A[min];
        A[min] = temp;
    }
}