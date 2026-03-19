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


    // -------- NEW FEATURE USAGE --------

    // Default vs value initialization
    int d;        // uninitialized (garbage value)
    int e{};      // initialized to 0

    std::cout << "e (value initialized) = " << e << "\n";

    // auto initialization
    auto f = 100;     // int
    auto g{200};      // int (uniform)

    std::cout << "f = " << f << ", g = " << g << "\n";

    // const and constexpr
    const int h = 50;
    constexpr int i = 60;

    std::cout << "h = " << h << ", i = " << i << "\n";

    // Array initialization
    int arr1[3] = {1, 2, 3};   // copy
    int arr2[3]{4, 5, 6};      // uniform

    std::cout << "arr1: ";
    for (int val : arr1) std::cout << val << " ";
    std::cout << "\n";

    std::cout << "arr2: ";
    for (int val : arr2) std::cout << val << " ";
    std::cout << "\n";

    // Narrowing conversion (will cause error if uncommented)
    // int bad{3.14}; // ❌ not allowed

    // ----------------------------------

    return 0;
}
