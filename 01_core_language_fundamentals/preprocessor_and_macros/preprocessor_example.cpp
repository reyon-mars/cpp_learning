// Preprocessor and Macros Exercise
// #define, #ifdef, include guards

#ifndef MY_HEADER_H
#define MY_HEADER_H

#include <iostream>

// ---------------- ORIGINAL MACROS ----------------
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define STRINGIFY(x) #x
#define CONCAT(a, b) a ## b
// ------------------------------------------------

// -------- SMALL ADDITIONS --------

// simple numeric macro
#define SQUARE(x) ((x) * (x))

// feature toggle macro
#define DEBUG_MODE

#ifdef DEBUG_MODE
    #define DEBUG_PRINT(msg) std::cout << "[DEBUG] " << msg << "\n"
#else
    #define DEBUG_PRINT(msg)
#endif

// ---------------------------------

int main() {

    std::cout << "Max of 5 and 10: " << MAX(5, 10) << "\n";
    std::cout << "Stringify: " << STRINGIFY(hello) << "\n";

    // ---- small added usage ----
    std::cout << "Square of 4: " << SQUARE(4) << "\n";

    DEBUG_PRINT("Debug mode is enabled");

    int CONCAT(var, 1) = 42;
    std::cout << "Concatenated variable value: " << var1 << "\n";
    // ---------------------------

    return 0;
}

#endif // MY_HEADER_H
