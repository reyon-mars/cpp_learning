#include <vector>
#include <climits>

int min_sum_subarray( const std::vector<int>& arr, int k ){
    if( arr.size() < k ){
        return 0;
    }
    int min_sum(INT_MAX), window(0);
    for( int i = 0; i < k; i++ ){
        window += arr[i];
    }
    min_sum = std::min( window, min_sum );
    for( int i = k; i < arr.size(); i++ ){
        window += arr[i] - arr[i-k];
        min_sum = std::min( window, min_sum );
    }
     return min_sum;
}