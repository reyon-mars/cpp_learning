#include <vector>

template< typename T>
T max_elem( std::vector<T> array ){
    
    if( array.empty() ){
        throw std::out_of_range();
    }
    T max = array[0];

    for( const auto& item: array ){
        max = ( max < item ) ? item : max ;    
    }
    return max;
};
