#include <iostream>
#include <vector>
#include <climits>

int max( int a, int b ){
    return a > b ? a : b;
}

int max_subarray_sum( const std::vector<int> arr, int k ){
    size_t n = arr.size();
    if( n < k ){ return -1; }

    int curr_win_sum(0), max_win_sum( INT_MIN );
    
    for( size_t i = 0; i < n; i++ ){
        curr_win_sum += arr[i];

        if( i >= k ){
            curr_win_sum -= arr[ i - k ];
        }
        if( i >= k - 1 ){
            max_win_sum = max( curr_win_sum, max_win_sum );
        }
    }
    return max_win_sum;
}