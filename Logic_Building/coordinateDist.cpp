#include <iostream>
#include <cmath>

struct Point{
    double x, y;
};

std::ostream& operator<<(std::ostream& out, const Point& p) {
    out << "( " << p.x << ", " << p.y << " )";
    return out;
}

double distance( Point p1, Point p2 ){
    return sqrt( pow( p1.x - p2.x, 2 ) + pow( p1.y - p2.y, 2 ));
}

int main(){
    Point p1, p2;

    while( true ){
        std::cout << "Please enter the coordinate for p1 : ";
        std::cin >> p1.x >> p1.y;
        std::cout << "Please enter the coorindate for p2 : ";
        std::cin >> p2.x >> p2.y;

        std::cout << "The distance between " << p1 << " and " << p2 << " is " << distance( p1, p2 );
        std::cout << std::endl;
    }
    return 0;
}