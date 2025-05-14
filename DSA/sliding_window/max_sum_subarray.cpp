#include <vector>

int max_sum_subarray( const std::vector<int>& arr , int k ){
    int max_sum(0), curr_sum(0);
    for( int i = 0; i < k; i++ ){
        curr_sum += arr[i];
    }
    max_sum = curr_sum;
    
    for( int i = k; i < arr.size(); i++ ){
        curr_sum += arr[i] - arr[i-k];
        max_sum = std::max( max_sum, curr_sum );
    }
    return max_sum;
}