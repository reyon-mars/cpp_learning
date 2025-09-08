#include <iostream>

std::string isPowerIter( int x, int y ){
    int expVal(1);
    if( y == 1 ){ return " is a power "; }
    for( int i = 0; i <= y/10; i++ ){
        expVal *= x;
        if( expVal == y ){
            return " is a power ";
        }
    }
    return " is not a power ";
}

int main(){

    int x(0), y(0);
    
    while( true ){
        std::cout << "Please enter the first number: ";
        std::cin >> x;
        std::cout << "Please enter the second number: ";
        std::cin >> y;

        std::cout << "The number " << y << isPowerIter( x, y ) << " of " << x << std::endl;
    }
    
}