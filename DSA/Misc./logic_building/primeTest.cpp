#include <iostream>
#include <string>
#include <cmath>

std::string isPrime( int num ){
    for( int i = 2; i <= sqrt(num); i++ ){
        if( ( num % i ) == 0 ){
            return " is not prime ";
        }
    }
    return " is prime ";
}

int main( ){
    int num(0);

    while(true){
        std::cout << "Please enter a number: ";
        std::cin >> num;

        std::cout << "The number " << num << isPrime( num ) << std::endl;
    }
    
    return 0;
}