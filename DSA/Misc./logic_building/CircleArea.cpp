#include <iostream>
#include <iomanip>
#define PI 3.14159265

int main(){

    int r(0);

    while( true ){
        std::cout << "Please enter the radius : ";
        std::cin >> r;

        double area = PI * r * r;

        std::cout << "The area of the circle with the radius " << r << " is "
                  << std::fixed << std::setprecision( 5 ) << area << std::endl;

    }

    return 0;
}