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

int main() {
    [[maybe_unused]] auto result = calculate();
    new_function();
    return 0;
}
