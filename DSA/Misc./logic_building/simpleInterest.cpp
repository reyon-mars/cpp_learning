#include <iostream>

int main( ){

    double p(0), r(0), t(0);

    while( true ){
        std::cout << "Please enter the value for p : ";
        std::cin >> p;
        std::cout << "Please enter the value for r : ";
        std::cin >> r;
        std::cout << "Please enter the value for t : ";
        std::cin >> t;

        double simpleInterest = ( p * t * r ) / 100;

        std::cout << "The simple interst is " << simpleInterest << std::endl;
    }

    return 0;
}