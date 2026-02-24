// Translation Units Exercise
// Demonstrates compilation model and separate translation units

#include <iostream>

// Forward declaration
void function_from_another_unit();

// ---------------- SMALL ADDITION ----------------
// (Normally this would live in another .cpp file)
void function_from_another_unit() {
    std::cout << "Function from another translation unit\n";
}
// ------------------------------------------------

int main() {
    std::cout << "Translation unit example\n";
    function_from_another_unit();
    return 0;
}
