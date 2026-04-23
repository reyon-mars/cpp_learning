#include <iostream>
#include <string>

// Put helpers in a small namespace (good practice)
namespace utils {

void greet() {
    std::cout << "Hello, User !" << std::endl;
}

inline int add(int a, int b) {
    return a + b;
}

// Simple Class
class Demo {
public:
    void show() const {   // const correctness
        std::cout << "Demo class method executed!" << std::endl;
    }
};

// Simple struct
struct Info {
    int value = 10;
};

// Simple helper function
inline void printMessage(const std::string& msg) {
    std::cout << msg << std::endl;
}

// Increment 
inline int increment(int x) {
    return x + 1;
}

inline bool isPositive(int x) {
    return x > 0;
}

// Check if number is even
inline bool isEven(int x) {
    return x % 2 == 0;
}

// Print divider line
inline void printDivider() {
    std::cout << "-----------------------------\n";
}

// Multiply two numbers
inline int multiply(int a, int b) {
    return a * b;
}

// Tiny stats struct
struct Stats {
    int runs = 1;
};

// Goodbye helper
inline void sayGoodbye() {
    std::cout << "Goodbye! End of program.\n";
}

// Square a number (constexpr example)
constexpr int square(int x) {
    return x * x;
}

// Simple status printer
inline void printStatus(const std::string& status) {
    std::cout << "[STATUS] " << status << std::endl;
}

// Tiny config struct
struct Config {
    bool debug = false;
};

// Check if number is zero
inline bool isZero(int x) {
    return x == 0;
}

// ---------------- SMALL ADDITIONS ----------------

// Toggle debug flag
inline void toggleDebug(Config& cfg) {
    cfg.debug = !cfg.debug;
}

// Increment stats runs
inline void recordRun(Stats& stats) {
    ++stats.runs;
}

// Absolute value helper
constexpr int absolute(int x) {
    return x < 0 ? -x : x;
}

// Template function (generic max)
template<typename T>
T maxValue(T a, T b) {
    return (a > b) ? a : b;
}

// Compile-time factorial
constexpr int factorial(int n) {
    return (n <= 1) ? 1 : n * factorial(n - 1);
}

// Convert string to uppercase
inline std::string toUpper(std::string str) {
    for (char& c : str) {
        if (c >= 'a' && c <= 'z') {
            c = c - 32;
        }
    }
    return str;
}

// Simple input validation
inline bool isValidNumber(int x) {
    return x >= 0 && x <= 100;
}


// Clamp value between range
inline int clamp(int value, int min, int max) {
    return (value < min) ? min : (value > max) ? max : value;
}

// Check if string is empty
inline bool isEmpty(const std::string& str) {
    return str.empty();
}

// Simple average
inline double average(int a, int b) {
    return (a + b) / 2.0;
}

} // namespace utils

// --------------------------------------------------
// MAIN
// --------------------------------------------------

int main(void) {

    using namespace utils;

    std::cout << "Hello, World" << std::endl;

    greet();
    std::cout << "5 + 7 = " << add(5, 7) << std::endl;

    Demo demo;
    demo.show();

    Info info;
    std::cout << "Info value = " << info.value << std::endl;

    printMessage("This is a simple helper function");

    int executionCount = 1;
    executionCount = increment(executionCount);

    std::cout << "Program execution count: "
              << executionCount << std::endl;

    std::cout << "Execution count is "
              << (isPositive(executionCount) ? "positive" : "not positive")
              << std::endl;

    std::cout << "Execution count is "
              << (isEven(executionCount) ? "even" : "odd")
              << std::endl;

    printDivider();

    Stats stats;
    recordRun(stats);
    std::cout << "Total runs recorded: "
              << stats.runs << std::endl;

    std::cout << "3 * 4 = " << multiply(3, 4) << std::endl;

    printStatus("Program running normally");

    Config cfg;
    toggleDebug(cfg);
    std::cout << "Debug mode: "
              << (cfg.debug ? "ON" : "OFF") << std::endl;

    std::cout << "Square of 5 = " << square(5) << std::endl;

    std::cout << "Execution count is "
              << (isZero(executionCount) ? "zero" : "not zero")
              << std::endl;

    std::cout << "Absolute of -7 = " << absolute(-7) << std::endl;

    // --------------------------------------------------


    std::cout << "Max of 10 and 20 = " << maxValue(10, 20) << std::endl;

    std::cout << "Factorial of 5 = " << factorial(5) << std::endl;

    std::cout << "Uppercase: " << toUpper("hello world") << std::endl;

    int testValue = 50;
    std::cout << "Is " << testValue << " valid? "
              << (isValidNumber(testValue) ? "Yes" : "No") << std::endl;

    std::cout << "Clamped value of 150 (0-100): "
              << clamp(150, 0, 100) << std::endl;

    std::cout << "Average of 10 and 20: "
              << average(10, 20) << std::endl;

    std::cout << "Is string empty? "
              << (isEmpty("") ? "Yes" : "No") << std::endl;

    // --------------------------------------------------

    std::cout << "Program finished successfully." << std::endl;
    sayGoodbye();

    return 0;
}
