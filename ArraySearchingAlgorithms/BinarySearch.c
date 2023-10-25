//Binary Search O(logn)

int binarySearch(int A[], int n, int k){
    
    int i = -1;
    int left = 0;
    int right = n-1;

    while(left <= right && i == -1){
        int mid = (left + right)/2;
        if(A[mid] == k){
            i = mid;
            return i;
        }
        else if(k < A[mid])
            right = mid-1;
        else
            left = mid+1;
    }
    return i;
}
