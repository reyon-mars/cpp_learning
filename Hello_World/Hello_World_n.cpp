#include <iostream>

int main( void ){
    int n (0);
    std::cout << "Please enter a number : ";
    std::cin >> n ;
    while ( n-- ){
        std::cout << "Hello World ! " << std::endl;
    }
    return 0;
}