// ----------- MORE ADVANCED ADDITIONS -----------

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


// ================= ADD IN MAIN =================

// (Add near the end before return)

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
