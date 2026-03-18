// Modules Exercise (C++20)
// Module interface and implementation units

// This would be in a module interface unit (.cppm)
// export module math_module;
// export int add(int a, int b) { return a + b; }

#include <iostream>

// ---- small added helper ----
void printModuleHint() {
    std::cout << "Modules replace headers in C++20\n";
}
// ----------------------------


// ----------- NEW ADDITIONS -----------

// Simulating module contents using namespace
namespace math_module_sim {

    // Simulated exported functions
    int add(int a, int b) {
        return a + b;
    }

    int subtract(int a, int b) {
        return a - b;
    }

    int multiply(int a, int b) {
        return a * b;
    }

    // Simulated exported constant
    const int version = 1;
}

// Function showing module-like usage
void useMathModule() {
    std::cout << "Add: "
              << math_module_sim::add(5, 3) << "\n";

    std::cout << "Subtract: "
              << math_module_sim::subtract(5, 3) << "\n";

    std::cout << "Multiply: "
              << math_module_sim::multiply(5, 3) << "\n";

    std::cout << "Module version: "
              << math_module_sim::version << "\n";
}

// ------------------------------------


int main() {

    std::cout << "C++20 Modules support\n";

    // ---- small added usage ----
    printModuleHint();
    // ---------------------------

    // -------- NEW FEATURE USAGE --------
    std::cout << "\nUsing simulated module:\n";
    useMathModule();
    // ----------------------------------

    return 0;
}
