#include <iostream>

double pow( double base, double power ){
    double res = 1;
    while( power ){
        res *= base;
        power--;
    }
    return res;
}

int main(){
    double a(0), r(0), n(0);

    while( true ){
        std::cout << "Please enter the value for a : ";
        std::cin >> a;
        std::cout << "Please enter the value for r : ";
        std::cin >> r;
        std::cout << "Please enter the value for n : ";
        std::cin >> n;

        double GPSum = ( a * ( 1 - ( pow( r, n ) ) ) )  / ( 1 - r );

        std::cout << "The Geometric sum is " << GPSum << std::endl;
    }
}