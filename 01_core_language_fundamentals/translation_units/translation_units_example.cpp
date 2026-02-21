// Translation Units Exercise
// Demonstrates compilation model and separate translation units

#include <iostream>

// Forward declaration
void function_from_another_unit();

int main() {
    std::cout << "Translation unit example\n";
    function_from_another_unit();
    return 0;
}
