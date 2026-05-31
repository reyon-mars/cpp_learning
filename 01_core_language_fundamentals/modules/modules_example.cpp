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
#include <vector>
#include <string>

// ---- small added helper ----
void printModuleHint() {
    std::cout << "Modules replace headers in C++20\n";
}
// ----------------------------


// ----------- NEW ADDITIONS -----------

namespace math_module_sim {

    int add(int a, int b) {
        return a + b;
    }

    int subtract(int a, int b) {
        return a - b;
    }

    int multiply(int a, int b) {
        return a * b;
    }

    const int version = 1;

    constexpr int square(int x) {
        return x * x;
    }

    struct Calculator {
        int add(int a, int b) const {
            return a + b;
        }
    };

    int divide(int a, int b) {
        if (b == 0) {
            std::cout << "Error: Division by zero\n";
            return 0;
        }
        return a / b;
    }

    int power(int base, int exp) {
        int result = 1;

        for (int i = 0; i < exp; ++i) {
            result *= base;
        }

        return result;
    }

    int factorial(int n) {
        if (n <= 1) return 1;
        return n * factorial(n - 1);
    }

    double average(const std::vector<int>& nums) {
        if (nums.empty()) return 0.0;

        int sum = 0;

        for (int v : nums) {
            sum += v;
        }

        return static_cast<double>(sum) / nums.size();
    }

    std::string moduleName() {
        return "math_module_sim";
    }

    // ------------------------------------------------
    // ✅ NEW ADDITIONS

    bool isPrime(int n) {

        if (n < 2) return false;

        for (int i = 2; i * i <= n; ++i) {
            if (n % i == 0)
                return false;
        }

        return true;
    }

    int gcd(int a, int b) {

        while (b != 0) {
            int temp = b;
            b = a % b;
            a = temp;
        }

        return a;
    }

    size_t featureCount() {
        return 10;
    }

    // ------------------------------------------------
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

    std::cout << "Square (constexpr): "
              << math_module_sim::square(5) << "\n";

    math_module_sim::Calculator calc;

    std::cout << "Calculator add: "
              << calc.add(10, 20) << "\n";

    std::cout << "Divide: "
              << math_module_sim::divide(10, 2) << "\n";

    std::cout << "Power(2, 5): "
              << math_module_sim::power(2, 5) << "\n";

    std::cout << "Factorial(5): "
              << math_module_sim::factorial(5) << "\n";

    std::vector<int> values = {10, 20, 30, 40};

    std::cout << "Average: "
              << math_module_sim::average(values) << "\n";

    std::cout << "Module Name: "
              << math_module_sim::moduleName() << "\n";

    // ✅ NEW
    std::cout << "Is 17 prime? "
              << (math_module_sim::isPrime(17) ? "Yes" : "No")
              << "\n";

    std::cout << "GCD(48, 18): "
              << math_module_sim::gcd(48, 18) << "\n";
}

// ✅ ADDED: simulate importing another module
namespace utility_module_sim {

    void printDivider() {
        std::cout << "-----------------------------\n";
    }

    void printStatus(const std::string& status) {
        std::cout << "[STATUS] " << status << "\n";
    }

    // ✅ NEW
    void printVector(const std::vector<int>& values) {

        for (int v : values) {
            std::cout << v << " ";
        }

        std::cout << "\n";
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

    // ✅ NEW TESTS

    std::cout << "Test Prime(11): "
              << (math_module_sim::isPrime(11) ? "Pass" : "Fail")
              << "\n";

    std::cout << "Test GCD(20,10): "
              << math_module_sim::gcd(20, 10) << "\n";

    utility_module_sim::printStatus("Tests completed");

    utility_module_sim::printDivider();
}

// ------------------------------------

int main() {

    std::cout << "C++20 Modules support\n";

    printModuleHint();

    std::cout << "\nUsing simulated module:\n";
    useMathModule();

    std::cout << "\nExecuting module test suite:\n";
    runModuleTests();

    constexpr int compileTimeSquare =
        math_module_sim::square(8);

    std::cout << "Compile-time square of 8: "
              << compileTimeSquare << "\n";

    std::cout << "Division by zero demo:\n";
    math_module_sim::divide(5, 0);

    std::cout << "\nNote:\n";
    std::cout << "Modules improve compile time and avoid header issues like multiple inclusion.\n";

    std::cout << "They also provide better encapsulation compared to traditional headers.\n";

    // ✅ NEW SECTION

    utility_module_sim::printDivider();

    std::cout << "Module Feature Summary:\n";

    std::cout << "Feature count: "
              << math_module_sim::featureCount()
              << "\n";

    std::vector<int> demoValues = {5, 10, 15, 20};

    std::cout << "Sample vector: ";
    utility_module_sim::printVector(demoValues);

    utility_module_sim::printDivider();

    std::cout << "\nFuture module topics:\n";
    std::cout << "- Real .cppm module interface files\n";
    std::cout << "- export/import syntax\n";
    std::cout << "- Partitioned modules\n";
    std::cout << "- Faster incremental builds\n";

    return 0;
}
