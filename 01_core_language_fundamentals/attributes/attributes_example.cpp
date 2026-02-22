// Attributes Exercise
// [[nodiscard]], [[deprecated]], [[maybe_unused]]

#include <iostream>

[[nodiscard]] int calculate() {
    return 42;
}

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

int main() {
    // nodiscard used correctly
    [[maybe_unused]] auto result = calculate();

    // deprecated function (kept for demonstration)
    // old_function();   // ⚠️ uncomment to see compiler warning

    new_function();

    debug_log(1);

    return 0;
}
