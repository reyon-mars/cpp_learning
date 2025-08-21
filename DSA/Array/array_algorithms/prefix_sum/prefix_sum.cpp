#include <vector>

int sum_range( std::vector<int> arr, int l, int r ){
    if( arr.empty() || l < 0 || r < 0 || l > r || l >= arr.size() || r >= arr.size() ){
        return -1;
    }
    std::vector<int> sum_vec( arr.size(), 0 );
    
    sum_vec[0] = arr[0];
    for( int i = 1; i < arr.size(); i++ ){
        sum_vec[i] = ( sum_vec[i - 1] + arr[i] );
    }
    
    return l == 0 ? sum_vec[r] : sum_vec[r] - sum_vec[l-1];
}