//Bubble Sort O(n^2)

void bubbleSort(int A[], int n){
    
    for(int i = 0 ; i < n ; i++){
        for(int j = 0 ; j < n-1 ; j++){
            int temp;
            if(A[j] > A[j+1]){
                temp = A[j];
                A[j] = A[j+1];
                A[j+1] = temp;
            }
        }
    }
}