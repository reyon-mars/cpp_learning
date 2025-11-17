#include <iostream>
#include <vector>
#include <algorithm>

int main ( void ){
    std::vector<int> vec;
    vec.reserve( 28 );

    for( int i = 0; i < 28; i++ ){
        vec.push_back(i);
    }
    std::cout << "Capacity" << vec.capacity() << '|' << "Size" << vec.size()<< std::endl;
    std::reverse( vec.begin(), vec.end() );
    
    for( const auto num: vec ){
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}