#include <iostream>

int factorial( int n ){
    if( n == 1 || n == 0 ){
        return 1;
    }
    return n * factorial( n - 1 );
}

int main(){
    int num(0);

    while( true ){
        std::cout << "Please enter a number : " ;
        std::cin >> num;
        std::cout << "The factorial of the number " << num << " is " << factorial(num) << std::endl;
        
    }
}