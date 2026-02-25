// Variables and Initialization Exercise
// Direct, copy, and uniform initialization

#include <iostream>

class Point {
public:
    int x, y;
    Point(int x = 0, int y = 0) : x(x), y(y) {}
};

// ---- small added helper ----
void print_point(const Point& p) {
    std::cout << "(" << p.x << ", " << p.y << ")\n";
}
// ----------------------------

int main() {
    int a = 10;  // Copy initialization
    int b(20);   // Direct initialization
    int c{30};   // Uniform initialization

    // ---- small added output ----
    std::cout << "a = " << a << "\n";
    std::cout << "b = " << b << "\n";
    std::cout << "c = " << c << "\n";
    // ----------------------------

    Point p1 = Point(1, 2);   // Copy initialization
    Point p2(3, 4);           // Direct initialization
    Point p3{5, 6};           // Uniform initialization

    // ---- small added output ----
    std::cout << "p1 = ";
    print_point(p1);

    std::cout << "p2 = ";
    print_point(p2);

    std::cout << "p3 = ";
    print_point(p3);
    // ----------------------------

    return 0;
}
