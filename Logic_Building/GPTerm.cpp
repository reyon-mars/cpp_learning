#include <iostream>

double pow( double base, double power ){

    if( power == 1 ){
        return base;
    }

    return base * pow( base, power - 1 );
}

int main(){

    double a(0), r(0), N(0);

    while( true ){
        std::cout << "Please enter the value for a : ";
        std::cin >> a;
        std::cout << "Please enter the value for r : ";
        std::cin >> r;
        std::cout << "Please enter the value for N : ";
        std::cin >> N;

        double NTerm = a * pow( r, N-1 );

        std::cout << "The " << N << " term of the Geometric Progression is " << NTerm << std::endl;
    }

    return 0;
}