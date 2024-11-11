//Insertion Sort O(n^2)

void insertionSort(int A[], int n) {
    int i = 0, j=0, k=0;
    
    for(j = 1 ; j < n ; j++) {
        k = A[j];
        i = j-1;
        while(i >= 0 && A[i] > k) {
            A[i+1] = A[i];
            -- i;
        }
        A[i+1] = k;
    }
}
