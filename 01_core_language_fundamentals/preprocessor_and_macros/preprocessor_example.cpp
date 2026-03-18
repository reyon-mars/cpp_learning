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


// ----------- NEW ADDITIONS -----------

// MIN macro
#define MIN(a, b) ((a) < (b) ? (a) : (b))

// Multi-line macro
#define PRINT_PAIR(a, b) \
    std::cout << "Values: " << a << ", " << b << "\n";

// Logging levels
#define LOG_LEVEL 2

#if LOG_LEVEL >= 2
    #define LOG_INFO(msg) std::cout << "[INFO] " << msg << "\n"
#else
    #define LOG_INFO(msg)
#endif

// Temporary macro to demonstrate undef
#define TEMP_MACRO 100

// ------------------------------------


int main() {

    std::cout << "Max of 5 and 10: " << MAX(5, 10) << "\n";
    std::cout << "Stringify: " << STRINGIFY(hello) << "\n";

    // ---- small added usage ----
    std::cout << "Square of 4: " << SQUARE(4) << "\n";

    DEBUG_PRINT("Debug mode is enabled");

    int CONCAT(var, 1) = 42;
    std::cout << "Concatenated variable value: " << var1 << "\n";
    // ---------------------------


    // -------- NEW FEATURE USAGE --------

    std::cout << "Min of 5 and 10: " << MIN(5, 10) << "\n";

    PRINT_PAIR(3, 7);

    LOG_INFO("This is an info log");

    std::cout << "Temp macro value: " << TEMP_MACRO << "\n";

    // Undefine macro
    #undef TEMP_MACRO

    // Uncommenting next line would cause error (macro removed)
    // std::cout << TEMP_MACRO;

    // ----------------------------------

    return 0;
}

#endif // MY_HEADER_H
