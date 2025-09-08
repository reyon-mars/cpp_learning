#include <iostream>

int sumOfDigits( int n ){

    if( n == 0 ){ return 0; }

    return ( n % 10 ) + sumOfDigits( n / 10 );
}

int main(){
    int n(0), num(0);
    while( true ){
        std::cout << "Please enter a number : ";
        std::cin >> n;
        num = n;

        int sum(0);

        while( n ){
            int digit = n % 10;
            sum += digit;
            n = n / 10;
        }
        std::cout << "The sum of the digits of " << num << " is " << sum << std::endl;

        std::cout << "The sum of the digits using recursive approach is " << sumOfDigits(num) << std::endl;
    }
}