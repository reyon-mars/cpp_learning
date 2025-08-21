
template< typename Iterator > 
void reverse( Iterator start, Iterator end ){
    
    --end;

    while( start < end ){
        std::swap( *start, *end );
        start++;
        end--;
    }
    
}