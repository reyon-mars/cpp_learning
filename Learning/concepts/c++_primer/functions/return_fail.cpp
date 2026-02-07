#include <iostream>
#include <cstdlib>

// ======================================================
// ORIGINAL MAIN CODE (UNCHANGED)
// ======================================================

int main(void) {
    return -1;
}

// ======================================================
// EXTRA CODE (SMALL & SIMPLE)
// ======================================================

// Simple extra function
void extraFunction() {
    std::cout << "Extra function executed before main." << std::endl;
}

// Small struct
struct ExtraStruct {
    int value = 10;
};

// ---- very small added helper ----
bool isPositive(int x) {
    return x > 0;
}

// ---- NEW SMALL HELPERS ----

// Check if number is even
bool isEven(int x) {
    return x % 2 == 0;
}

// Print separator line
void printLine() {
    std::cout << "----------------------" << std::endl;
}

// Small counter struct
struct Counter {
    int count = 1;
};

// Runs before main()
int runExtra() {
    ExtraStruct e;
    std::cout << "ExtraStruct value: " << e.value << std::endl;

    std::cout << "Value is "
              << (isPositive(e.value) ? "positive" : "not positive")
              << std::endl;

    std::cout << "Value is "
              << (isEven(e.value) ? "even" : "odd")
              << std::endl;

    Counter c;
    std::cout << "Counter value: " << c.count << std::endl;

    printLine();
    extraFunction();
    return 0;
}

// ---- VERY SMALL EXTRA ADDITION ----

// Runs after main()
void runAfterMain() {
    std::cout << "Program exited. Cleanup complete." << std::endl;
}

// Global initialization (executes before main)
static int init_guard = runExtra();

// Register after-main function
static int atexit_guard = std::atexit(runAfterMain);
