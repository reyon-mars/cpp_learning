#include <iostream>

int sumNatural( int n ){
    if ( n == 1 ){ return 1; }
    return n + sumNatural( n - 1 );
}

int main(){
    int n(0);

    while( true ){
        std::cout << "Please enter a number: ";
        std::cin >> n;

        std::cout << "The sum of " << n << " natural number is " 
                  << sumNatural(n) << std::endl;
    }
    
}