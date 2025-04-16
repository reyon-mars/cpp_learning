#include <iostream>
#include <string>

double max( double a, double b ){
    return a > b ? a : b;
}

std::string isValidTriangle( double x, double y, double z ){
    double longestSide = max( x, max( y, z ) );
    if( longestSide < (( x + y + z ) - longestSide) ){
        return " is a valid triangle ";
    }
    return " is not a valid triangle ";
}

int main( ){
    double a, b, c;
    
    std::cout << "Enter the three sides of a triangle: ";
    std::cin >> a >> b >> c;

    std::cout << "Triangle with sides (" << a << ", " << b << ", " << c << ") " << isValidTriangle( a, b, c ) << std::endl;
}