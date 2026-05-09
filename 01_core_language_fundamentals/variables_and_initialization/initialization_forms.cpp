#include <iostream>         // ✅ ADDED
#include <initializer_list> // ✅ ADDED
#include <vector>           // ✅ NEW
#include <cassert>          // ✅ NEW
#include <type_traits>      // ✅ NEW

// ✅ ADDED: Point struct
struct Point {
    int x;
    int y;
};

// ✅ ADDED: Print function
void print_point(const Point& p) {
    std::cout << "(" << p.x << ", " << p.y << ")\n";
}

// Class to demonstrate explicit constructor
class ExplicitDemo {
public:
    explicit ExplicitDemo(int v) : value(v) {}
    int value;
};

// Function to show initialization difference
void show_init_difference() {
    int x(5);   // direct
    int y{5};   // uniform

    std::cout << "x = " << x << ", y = " << y << "\n";
}

// ----------------------------------------------
// ✅ NEW SMALL ADDITIONS
// ----------------------------------------------

// Aggregate initialization
struct Student {
    std::string name;
    int age;
};

// Print student
void print_student(const Student& s) {
    std::cout << "Student: "
              << s.name
              << ", age "
              << s.age << "\n";
}

// Constructor initialization list
class Box {
private:
    int width;
    int height;

public:
    Box(int w, int h)
        : width(w), height(h) {}

    void show() const {
        std::cout << "Box("
                  << width
                  << ", "
                  << height
                  << ")\n";
    }
};

// Narrowing prevention demo
void narrowing_demo() {

    int a = 3.14; // allowed (narrowing)

    std::cout << "Traditional narrowing value: "
              << a << "\n";

    // int b{3.14}; // ❌ compile-time error

    std::cout << "Uniform initialization prevents narrowing.\n";
}

// Initializer list constructor
class NumberList {
public:
    NumberList(std::initializer_list<int> vals)
        : values(vals) {}

    void print() const {
        for (int v : values) {
            std::cout << v << " ";
        }
        std::cout << "\n";
    }

private:
    std::vector<int> values;
};

// constexpr initialized object
struct Config {
    int version;
    bool debug;
};

// ----------------------------------------------

int main() {

    std::cout << "\nAdvanced Initialization Concepts:\n";

    // ✅ Most vexing parse (function declaration instead of object)
    // Point tricky(); // ⚠️ this declares a function, not an object!

    // ✅ Zero initialization for objects
    Point p4{}; // (0,0)
    std::cout << "p4 (zero-initialized) = ";
    print_point(p4);

    // ✅ Explicit constructor demo
    ExplicitDemo ex1(10);   // OK
    // ExplicitDemo ex2 = 10; // ❌ not allowed due to explicit

    std::cout << "ExplicitDemo value: " << ex1.value << "\n";

    // ✅ auto with initializer list
    auto list = {1, 2, 3}; // std::initializer_list<int>
    std::cout << "Initializer list size: " << list.size() << "\n";

    // ✅ difference demonstration
    show_init_difference();

    // --------------------------------------------------
    // ✅ NEW FEATURE USAGE
    // --------------------------------------------------

    // Aggregate initialization
    Student s1{"Alice", 20};
    print_student(s1);

    // Constructor initialization list
    Box b1(5, 10);
    b1.show();

    // Narrowing demo
    narrowing_demo();

    // Initializer list constructor
    NumberList nums{1, 2, 3, 4, 5};

    std::cout << "NumberList values: ";
    nums.print();

    // constexpr object
    constexpr Config cfg{2, true};

    std::cout << "Config version: "
              << cfg.version << "\n";

    std::cout << "Debug mode: "
              << (cfg.debug ? "ON" : "OFF") << "\n";

    // Type deduction check
    static_assert(
        std::is_same<
            decltype(list),
            std::initializer_list<int>
        >::value,
        "list should be initializer_list<int>"
    );

    // Assertion demo
    assert(ex1.value == 10);

    // Dynamic initialization
    std::vector<Point> points = {
        {1, 2},
        {3, 4},
        {5, 6}
    };

    std::cout << "\nVector of points:\n";

    for (const auto& p : points) {
        print_point(p);
    }

    // --------------------------------------------------

    return 0;
}
