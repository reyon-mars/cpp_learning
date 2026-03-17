// Compilation Model Exercise
// Understanding preprocessing, parsing, compilation, and linking

#include <iostream>

// ---------- SMALL ADDITIONS ----------

// Preprocessor macro
#define APP_VERSION "1.0"

// Conditional compilation
#define DEBUG_MODE

// Function prototype (parsing stage example)
void debugMessage();

// Simple helper function (compiled separately conceptually)
void printVersion() {
    std::cout << "App Version: " << APP_VERSION << "\n";
}

// Global variable (linking example)
int build_number = 1;

// Static variable (internal linkage)
static int internal_counter = 0;

// Inline function (compiler optimization)
inline int square(int x) {
    return x * x;
}

// Function definition (after prototype)
void debugMessage() {
#ifdef DEBUG_MODE
    std::cout << "[DEBUG] Debug mode is ON\n";
#endif
}

// Simulating extern usage
extern int build_number;

// ------------------------------------

int main() {
    std::cout << "Compiled successfully\n";

    // Added usage
    printVersion();
    std::cout << "Build number: " << build_number << "\n";

    // -------- NEW ADDITIONS USAGE --------
    debugMessage();

    internal_counter++;
    std::cout << "Internal counter: " << internal_counter << "\n";

    std::cout << "Square of 5: " << square(5) << "\n";
    // ------------------------------------

    return 0;
}
