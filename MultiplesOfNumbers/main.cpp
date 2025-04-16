#include <iostream>

int main( void ){
    while( true ){
        int N(0);
        std::cout << "Please enter a number: ";
        std::cin >> N;
        if( N < 0 ){ 
            std::cout << "Goodbye !" << std::endl;
            break; 
            }
        if( N >= 0 && N % 5 != 0 ){continue;}
        std::cout << (( N >= 0 && N % 5 == 0 ) ?
                      (N / 5) : -1) << std::endl;
    }
}