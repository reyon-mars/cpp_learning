#include <iostream>

int main( void ) {
    int n(0);
    std::cout << "Please enter a number: ";
    std::cin >> n;
    constexpr const char* str = " Hello, World !";
    for( int  count = 0; count < n ; count++ ){
        std::cout << str << "\n";
    }
    
    return 0;
}