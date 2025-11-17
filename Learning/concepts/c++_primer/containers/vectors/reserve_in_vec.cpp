#include <iostream>
#include <vector>

int main ( void ){
    std::vector<int> vec;
    vec.reserve( 28 );

    for( int i = 0; i < 28; i++ ){
        vec.push_back(i);
    }
    std::cout << "Capacity" << vec.capacity() << '|' << "Size" << vec.size()<< std::endl;

    return 0;
}