//Counting Sort O(n+k)

void countingSort(int A[], int B[], int k, int n) {
    int C[k+1];
    for(int i = 0 ; i <= k ; i ++)
        C[i] = 0;
    for(int j = 0 ; j < n ; j++)
        C[A[j]]++;

    int j = 0;
    for(int z = 0 ; z <= k ; z++) {
        for(int v = 0 ; v < C[z] ; v++) {
            B[j] = z;
            j++;
        }
    }
}