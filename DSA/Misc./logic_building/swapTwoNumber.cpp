#include <iostream>

void swap( int& a, int& b ){
    a = a ^ b;
    b = b ^ a;
    a = a ^ b;
}

int main( ){
    int a(0) , b(0);

    while(true){    
        std::cout << "Please enter the first number: ";
        std::cin >> a;
        std::cout << "Please enter the second number: ";
        std::cin >> b;

        std::cout << "a = " << a << " b = " << b << std::endl;
        swap(a, b);
        std::cout << "a = " << a << " b = " << b << std::endl;
    }
    return 0;

}