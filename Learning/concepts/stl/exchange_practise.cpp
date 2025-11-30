#include <iostream>
#include <utility>

class counter{
    private:
    int value;

    public:

    int increment( int amount ){
        return std::exchange( value, value + amount );
    }

    int reset ( ){
        return std::exchange( value, 0 );
    }
};


int main ( void ){

    counter countr;
    for( int i = 1; i < 100; i++ ){
        std::cout << countr.increment( i ) << " ";
    }
    std::cout << countr.reset(); 
    return 0;

}

