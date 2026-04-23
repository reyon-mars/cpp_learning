// Compilation Model Exercise
// Understanding preprocessing, parsing, compilation, and linking

#include <iostream>

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


// Safer alternative to macro (inline function)
inline int max_safe(int a, int b) {
    return (a > b) ? a : b;
}

// Macro with file and line info
#define DEBUG_LOG(msg) \
    std::cout << "[DEBUG] " << msg \
              << " (" << __FILE__ << ":" << __LINE__ << ")\n"

// Conditional compilation using defined
#if defined(DEBUG_MODE)
    #define MODE_STATUS "Debug Mode Active"
#else
    #define MODE_STATUS "Release Mode"
#endif

// Macro pitfall example
#define BAD_SQUARE(x) x * x  // missing parentheses!

// ----------------------------------------------


int main() {
    std::cout << "Compiled successfully\n";

    // Added usage
    printVersion();
    std::cout << "Build number: " << build_number << "\n";

    debugMessage();

    internal_counter++;
    std::cout << "Internal counter: " << internal_counter << "\n";

    std::cout << "Square of 5: " << square(5) << "\n";

    // Macro vs constexpr
    std::cout << "Square (macro): " << SQUARE_MACRO(5) << "\n";
    std::cout << "Square (constexpr): " << square_constexpr(5) << "\n";

    // Config usage
    std::cout << "Max users (macro): " << MAX_USERS << "\n";

    // External function simulation
    externalFunction();



    std::cout << "\nAdvanced Macro Features:\n";

    std::cout << "max_safe(5,10): " << max_safe(5,10) << "\n";

    DEBUG_LOG("Testing debug log");

    std::cout << "Mode: " << MODE_STATUS << "\n";

    std::cout << "BAD_SQUARE(2+3): " << BAD_SQUARE(2+3) << "\n";
    std::cout << "Correct SQUARE(2+3): " << SQUARE_MACRO(2+3) << "\n";

    // undef example
    #undef DEBUG_MODE
    // debugMessage(); // would not print now if called again

    return 0;
}
