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


// ----------- NEW ADDITIONS -----------

// Simulating another function from different unit
void another_external_function() {
    std::cout << "Another external function\n";
}

// Static function (internal linkage)
static void internal_helper() {
    std::cout << "Internal helper (only visible in this file)\n";
}

// Simulated extern variable
int shared_value = 100;
extern int shared_value;

// Function that uses external-like features
void use_external_features() {
    std::cout << "Shared value: " << shared_value << "\n";
    another_external_function();
}

// ------------------------------------


int main() {
    std::cout << "Translation unit example\n";
    function_from_another_unit();

    // -------- NEW FEATURE USAGE --------
    internal_helper();
    use_external_features();
    // ----------------------------------

    return 0;
}
