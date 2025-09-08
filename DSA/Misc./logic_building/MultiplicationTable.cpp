#include <iostream>

int main( ){
    int n(0);
    std::cout << "Please enter a number: ";
    std::cin >> n;

    for( int i = 1; i <= 12; i++ ){
        std::cout << n << "x" << i << " = " << n * i << std::endl;
    }

    return 0;
}