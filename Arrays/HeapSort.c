//Heap Sort O(nlogn)

void maxHipify(int A[], int i, int n) {
    int l = 2*i;
    int r = (2*i)+1;
    int max = i;

    if(l < n && A[l] > A[i])
        max = l;
    if(l < n && A[r] > A[max])
        max = r;
    if(max != i) {
        int temp = A[i];
        A[i] = A[max];
        A[max] = temp;
        maxHipify(A,max,n);
    }
}

void buildMaxHeap(int A[],int n) {
    for(int i = n/2 ; i > 0 ; --i){
        maxHipify(A,i,n);
    }
}

void heapSort(int A[],int n) {
    int heapSize = n;
    buildMaxHeap(A,n);

    for(int i = n ; i > 1 ; --i) {
        int temp = A[0];
        A[0] = A[i];
        A[i] = temp;
        heapSize--;
        maxHipify(A,0,heapSize);
    }
}