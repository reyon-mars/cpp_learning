#include <iostream>

int squareSumNatural( int n ){
    return n * ( n + 1 ) * ( 2*n + 1 ) / 6;
}

int main(){
    int input(0);
    
    while( true ){
        std::cout << "Please enter a natural number: ";
        std::cin >> input;

        std::cout << "The sum of square of number till " << input << " is " << squareSumNatural( input );
        std::cout << std::endl;
    }

    return 0;
}