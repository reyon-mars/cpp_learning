// Compilation Model Exercise
// Understanding preprocessing, parsing, compilation, and linking

#include <iostream>

// ---------- SMALL ADDITIONS ----------

// Preprocessor macro
#define APP_VERSION "1.0"

// Simple helper function (compiled separately conceptually)
void printVersion() {
    std::cout << "App Version: " << APP_VERSION << "\n";
}

// Global variable (linking example)
int build_number = 1;

// ------------------------------------

int main() {
    std::cout << "Compiled successfully\n";

    // Added usage
    printVersion();
    std::cout << "Build number: " << build_number << "\n";

    return 0;
}
