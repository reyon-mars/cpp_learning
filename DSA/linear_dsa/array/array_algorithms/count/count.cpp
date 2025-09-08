template< typename T>
int count( std::vector<T> arr, T target ){
    int freq(0);
    for( size_t i = 0; i < arr.size(); i++ )
        if( arr[i] == target )
            freq++;
    return freq;
};