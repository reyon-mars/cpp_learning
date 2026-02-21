// Variables and Initialization Exercise
// Direct, copy, and uniform initialization

#include <iostream>

class Point {
public:
    int x, y;
    Point(int x = 0, int y = 0) : x(x), y(y) {}
};

int main() {
    int a = 10;  // Copy initialization
    int b(20);   // Direct initialization
    int c{30};   // Uniform initialization
    
    Point p1 = Point(1, 2);   // Copy initialization
    Point p2(3, 4);           // Direct initialization
    Point p3{5, 6};           // Uniform initialization
    
    return 0;
}
