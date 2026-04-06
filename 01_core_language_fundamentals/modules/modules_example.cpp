// Modules Exercise (C++20)
// Module interface and implementation units

// This would be in a module interface unit (.cppm)
// export module math_module;
// export int add(int a, int b) { return a + b; }

// ✅ ADDED: More realistic module example (commented)
/*
export module math_module;

export namespace math {
    int add(int a, int b);
    int multiply(int a, int b);
}
*/

// Implementation unit (.cpp)
/*
module math_module;

int math::add(int a, int b) {
    return a + b;
}

int math::multiply(int a, int b) {
    return a * b;
}
*/

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

    // ✅ ADDED: constexpr (compile-time function)
    constexpr int square(int x) {
        return x * x;
    }

    // ✅ ADDED: struct like exported type
    struct Calculator {
        int add(int a, int b) const {
            return a + b;
        }
    };
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

    // ✅ ADDED usage
    std::cout << "Square (constexpr): "
              << math_module_sim::square(5) << "\n";

    math_module_sim::Calculator calc;
    std::cout << "Calculator add: "
              << calc.add(10, 20) << "\n";
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

    // ✅ ADDED: explanation output
    std::cout << "\nNote:\n";
    std::cout << "Modules improve compile time and avoid header issues like multiple inclusion.\n";

    return 0;
}
