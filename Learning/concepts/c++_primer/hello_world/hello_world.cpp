#include <iostream>
#include <string>

// ======================================================
// ORIGINAL CODE (LOGIC UNCHANGED)
// ======================================================

void greet() {
    std::cout << "This is extra code added without modifying main!" << std::endl;
}

int add(int a, int b) {
    return a + b;
}

class Demo {
public:
    void show() {
        std::cout << "Demo class method executed!" << std::endl;
    }
};

// ======================================================
// SMALL ADDITIONAL CODE
// ======================================================

// Simple struct
struct Info {
    int value = 10;
};

// Simple helper function
void printMessage(const std::string& msg) {
    std::cout << msg << std::endl;
}

// Simple class
class ExtraDemo {
public:
    void run() {
        std::cout << "ExtraDemo running successfully!" << std::endl;
    }
};

// Very small helper
int increment(int x) {
    return x + 1;
}

// Another tiny helper
bool isPositive(int x) {
    return x > 0;
}

// ---- VERY SMALL EXTRA HELPERS ----

// Check if number is even
bool isEven(int x) {
    return x % 2 == 0;
}

// Print divider line
void printDivider() {
    std::cout << "-----------------------------\n";
}

// Multiply two numbers
int multiply(int a, int b) {
    return a * b;
}

// Tiny stats struct
struct Stats {
    int runs = 1;
};

// Goodbye helper
void sayGoodbye() {
    std::cout << "Goodbye! End of program.\n";
}
// --------------------------------

// ======================================================
// MAIN
// ======================================================

int main(void) {

    // ---------- ORIGINAL ----------
    std::cout << "Hello, World" << std::endl;

    greet();
    std::cout << "5 + 7 = " << add(5, 7) << std::endl;

    Demo demo;
    demo.show();

    // ---------- SMALL EXTRA ----------
    Info info;
    std::cout << "Info value = " << info.value << std::endl;

    printMessage("This is a simple helper function");

    ExtraDemo extra;
    extra.run();

    // ---------- VERY SMALL ADDITION ----------
    int executionCount = 1;
    executionCount = increment(executionCount);

    std::cout << "Program execution count: "
              << executionCount << std::endl;

    std::cout << "Execution count is "
              << (isPositive(executionCount) ? "positive" : "not positive")
              << std::endl;

    // ---- tiny added usage ----
    std::cout << "Execution count is "
              << (isEven(executionCount) ? "even" : "odd")
              << std::endl;

    printDivider();

    Stats stats;
    std::cout << "Total runs recorded: "
              << stats.runs << std::endl;

    std::cout << "3 * 4 = " << multiply(3, 4) << std::endl;
    // --------------------------

    std::cout << "Program finished successfully." << std::endl;
    sayGoodbye();
    return 0;
}
