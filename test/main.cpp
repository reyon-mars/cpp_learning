#include <iostream>

int main( void ) {
    
    short number; 
    std::cout << "Enter a number : ";
    std::cin >> number; 

    std::cout << "The factorial of " << number << " is ";
    int accumulator = 1; 
    for( ; number > 0; accumulator *= number--);
    std::cout << accumulator << ".\n";

    return 0;
}