#include <vector>


template< typename T>
T find_elem( std::vector<T>& array, bool (*cmpr)( T, T ) ){
    
    if( array.empty() ){
        throw std::out_of_range( "The array is emtpy." );
    }

    T result = array[0];
    for( const auto& item: array ){
        if( compr( result, item ) ){
            result = item;
        }   
    }
    return result;
};
