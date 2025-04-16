#include <iostream>

bool odd( int num ){
    return ( num & 1 ) ? true : false; 
}

int main( ){

    for ( int i = 0; i < 10; i++ ){
        std::cout << i << " " << odd( i ) << std::endl;
    }

    return 0;
}