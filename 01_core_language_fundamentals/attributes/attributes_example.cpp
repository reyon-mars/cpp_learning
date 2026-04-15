// Attributes Exercise
// [[nodiscard]], [[deprecated]], [[maybe_unused]]

#include <iostream>

// ✅ ADDED
#include <utility>   // for structured binding pair

[[nodiscard]] int calculate() {
    return 42;
}

// Apply nodiscard to a type
struct [[nodiscard]] ImportantResult {
    int value;
};

[[deprecated("Use new_function instead")]]
void old_function() {
    std::cout << "Old function\n";
}

void new_function() {
    std::cout << "New function\n";
}

// maybe_unused parameter demo
void debug_log([[maybe_unused]] int level) {
    // intentionally unused
}

// maybe_unused function
[[maybe_unused]] void helper() {
    std::cout << "Helper function\n";
}

// ✅ ADDED: nodiscard with message (C++20)
[[nodiscard("You must use the return value")]]
int compute_value() {
    return 99;
}

// ✅ ADDED: nodiscard method inside class
class Calculator {
public:
    [[nodiscard]] int add(int a, int b) const {
        return a + b;
    }
};

// ✅ ADDED: deprecated class
[[deprecated("Use NewSystem instead")]]
class OldSystem {
public:
    void run() {
        std::cout << "Running old system\n";
    }
};

class NewSystem {
public:
    void run() {
        std::cout << "Running new system\n";
    }
};

// ✅ ADDED: helper to demonstrate nodiscard usage properly
void use_result(int value) {
    std::cout << "Using result: " << value << "\n";
}

int main() {

    // nodiscard used correctly
    [[maybe_unused]] auto result = calculate();

    // ✅ ADDED: Proper usage example
    use_result(calculate());

    // Uncomment to trigger nodiscard warning:
    // calculate();  // ⚠️ compiler may warn: return value ignored

    // Using nodiscard type
    ImportantResult res{100};
    std::cout << "ImportantResult: " << res.value << "\n";

    // deprecated function (kept for demonstration)
    // old_function();   // ⚠️ uncomment to see compiler warning

    new_function();

    debug_log(1);

    // maybe_unused variable
    [[maybe_unused]] int temp = 10;

    // ----------------------------------------------------
    // ✅ ADDED: nodiscard with message
    [[maybe_unused]] int val = compute_value();
    std::cout << "Computed value: " << val << "\n";

    // ----------------------------------------------------
    // ✅ ADDED: class method nodiscard
    Calculator calc;
    [[maybe_unused]] int sum = calc.add(5, 3);

    std::cout << "Sum: " << sum << "\n";

    // ----------------------------------------------------
    // ✅ ADDED: deprecated class usage
    // OldSystem oldSys;  // ⚠️ will show warning if uncommented
    // oldSys.run();

    NewSystem newSys;
    newSys.run();

    // ----------------------------------------------------
    // ✅ ADDED: structured binding with maybe_unused
    std::pair<int, int> p = {1, 2};
    auto [x, [[maybe_unused]] y] = p;

    std::cout << "Structured binding x: " << x << "\n";

    // ----------------------------------------------------
    // ✅ ADDED: Explicit ignore pattern
    (void)calculate(); // intentional ignore (no warning in some compilers)

    return 0;
}
