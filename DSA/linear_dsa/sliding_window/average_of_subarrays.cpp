#include <vector>

std::vector<float> average_of_subarrays( const std::vector<int>& arr , int k ){
    if( k > arr.size() ){
        return {};
    }
    std::vector<float> result;
    result.reserve(arr.size()-k+1);
    int window_sum(0);
    for( int i = 0; i < k; i++ ){
         window_sum += arr[i];
    }
    result.push_back( window_sum/float(k));
    for( int i = k; i < arr.size(); i++ ){
        window_sum += arr[i] - arr[i-k];
        result.push_back( window_sum/float(k));
    }
    return result;
}