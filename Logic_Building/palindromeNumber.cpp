#include <iostream>

long long reverseNumber( long long number ){
    long long result(0);

    while( number ){
        short digit = number % 10;
        result = result * 10 + digit;
        number /= 10; 
    }
    return result;
}

int main(){
    long long input(0);
    
    while( true ){
        std::cout << "Please enter a number: ";
        std::cin >> input;
        
        if( input == reverseNumber( input ) ){
            std::cout << input << " is a palindrome number. " << std::endl;
        }else {
            std::cout << input << " is not a palindrome number. " << std::endl;
        }
    }
    return 0;
}