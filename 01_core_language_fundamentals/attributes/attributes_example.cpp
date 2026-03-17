// Attributes Exercise
// [[nodiscard]], [[deprecated]], [[maybe_unused]]

#include <iostream>

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

int main() {

    // nodiscard used correctly
    [[maybe_unused]] auto result = calculate();

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

    return 0;
}
