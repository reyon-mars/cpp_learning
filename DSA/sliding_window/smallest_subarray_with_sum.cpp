#include <vector>
#include <climits>

int smallest_subarray_with_sum( const std::vector<int>& arr , int k ){
    int left(0), right(0), curr_sum(0), min_len(INT_MAX);
    
    while( right < arr.size() ){

        curr_sum += arr[right++];
        
        while( curr_sum >= k ){
            curr_sum -= arr[left++];
            min_len = std::min( min_len, right- left);
        }
    }
    return min_len;
}