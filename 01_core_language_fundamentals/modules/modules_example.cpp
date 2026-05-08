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
#include <vector>     // ✅ ADDED
#include <string>     // ✅ ADDED

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

    // ✅ ADDED: division with safety
    int divide(int a, int b) {
        if (b == 0) {
            std::cout << "Error: Division by zero\n";
            return 0;
        }
        return a / b;
    }

    // ✅ ADDED: power function
    int power(int base, int exp) {
        int result = 1;

        for (int i = 0; i < exp; ++i) {
            result *= base;
        }

        return result;
    }

    // ✅ ADDED: factorial
    int factorial(int n) {
        if (n <= 1) return 1;
        return n * factorial(n - 1);
    }

    // ✅ ADDED: average helper
    double average(const std::vector<int>& nums) {
        if (nums.empty()) return 0.0;

        int sum = 0;

        for (int v : nums) {
            sum += v;
        }

        return static_cast<double>(sum) / nums.size();
    }

    // ✅ ADDED: module info
    std::string moduleName() {
        return "math_module_sim";
    }
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

    // ✅ ADDED: division usage
    std::cout << "Divide: "
              << math_module_sim::divide(10, 2) << "\n";

    // ✅ ADDED: power usage
    std::cout << "Power(2, 5): "
              << math_module_sim::power(2, 5) << "\n";

    // ✅ ADDED: factorial usage
    std::cout << "Factorial(5): "
              << math_module_sim::factorial(5) << "\n";

    // ✅ ADDED: average usage
    std::vector<int> values = {10, 20, 30, 40};

    std::cout << "Average: "
              << math_module_sim::average(values) << "\n";

    // ✅ ADDED: module name
    std::cout << "Module Name: "
              << math_module_sim::moduleName() << "\n";
}

// ✅ ADDED: simulate importing another module
namespace utility_module_sim {

    void printDivider() {
        std::cout << "-----------------------------\n";
    }

    void printStatus(const std::string& status) {
        std::cout << "[STATUS] " << status << "\n";
    }
}

// ✅ ADDED: testing helper
void runModuleTests() {

    utility_module_sim::printDivider();

    std::cout << "Running simulated module tests...\n";

    std::cout << "Test Add: "
              << math_module_sim::add(1, 2) << "\n";

    std::cout << "Test Multiply: "
              << math_module_sim::multiply(3, 4) << "\n";

    std::cout << "Test Power: "
              << math_module_sim::power(3, 3) << "\n";

    utility_module_sim::printStatus("Tests completed");

    utility_module_sim::printDivider();
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

    // ✅ ADDED: module test runner
    std::cout << "\nExecuting module test suite:\n";
    runModuleTests();

    // ✅ ADDED: constexpr compile-time example
    constexpr int compileTimeSquare = math_module_sim::square(8);

    std::cout << "Compile-time square of 8: "
              << compileTimeSquare << "\n";

    // ✅ ADDED: safe division demo
    std::cout << "Division by zero demo:\n";
    math_module_sim::divide(5, 0);

    // ✅ ADDED: explanation output
    std::cout << "\nNote:\n";
    std::cout << "Modules improve compile time and avoid header issues like multiple inclusion.\n";

    // ✅ ADDED: extra clarification
    std::cout << "They also provide better encapsulation compared to traditional headers.\n";

    // ✅ ADDED: future learning topics
    std::cout << "\nFuture module topics:\n";
    std::cout << "- Real .cppm module interface files\n";
    std::cout << "- export/import syntax\n";
    std::cout << "- Partitioned modules\n";
    std::cout << "- Faster incremental builds\n";

    return 0;
}
