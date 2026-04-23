// Compilation Model Exercise
// Understanding preprocessing, parsing, compilation, and linking

#include <iostream>

// ---------- SMALL ADDITIONS ----------

// Preprocessor macro
#define APP_VERSION "1.0"

// Conditional compilation
#define DEBUG_MODE

// Header guard simulation
#ifndef CONFIG_H
#define CONFIG_H
#define MAX_USERS 100
#endif

// Macro function
#define SQUARE_MACRO(x) ((x) * (x))

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

// constexpr alternative (better than macro)
constexpr int square_constexpr(int x) {
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

// extern function simulation
void externalFunction() {
    std::cout << "Simulated external function call\n";
}


// Macro pitfall example
#define DOUBLE_MACRO(x) x + x

// constexpr safe version
constexpr int double_safe(int x) {
    return x + x;
}

// Static function (internal linkage demo)
static void internalHelper() {
    std::cout << "Internal helper function\n";
}

// ------------------------------------------------------

int main() {
    std::cout << "Compiled successfully\n";

    printVersion();
    std::cout << "Build number: " << build_number << "\n";

    debugMessage();

    internal_counter++;
    std::cout << "Internal counter: " << internal_counter << "\n";

    std::cout << "Square of 5: " << square(5) << "\n";

    std::cout << "Square (macro): " << SQUARE_MACRO(5) << "\n";
    std::cout << "Square (constexpr): " << square_constexpr(5) << "\n";

    std::cout << "Max users (macro): " << MAX_USERS << "\n";

    externalFunction();


    // Macro pitfall demo
    std::cout << "DOUBLE_MACRO(5*2): "
              << DOUBLE_MACRO(5 * 2)   // expands wrongly → 5*2 + 5*2
              << "\n";

    std::cout << "double_safe(5*2): "
              << double_safe(5 * 2)
              << "\n";

    internalHelper();

    // Compile-time constant usage
    constexpr int val = square_constexpr(6);
    std::cout << "Compile-time evaluated value: " << val << "\n";

    // --------------------------------------------

    // undef example
    #undef DEBUG_MODE
    // debugMessage(); // would not print now if called again

    return 0;
}
