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

// Runs before main()
int runExtra() {
    ExtraStruct e;
    std::cout << "ExtraStruct value: " << e.value << std::endl;

    // small added usage
    std::cout << "Value is "
              << (isPositive(e.value) ? "positive" : "not positive")
              << std::endl;

    extraFunction();
    return 0;
}

// ---- VERY SMALL EXTRA ADDITION ----

// Runs after main()
void runAfterMain() {
    std::cout << "Program exited. Cleanup complete." << std::endl;
}

// Global initialization (executes before main)
int _ = runExtra();

// Register after-main function
int __ = std::atexit(runAfterMain);
