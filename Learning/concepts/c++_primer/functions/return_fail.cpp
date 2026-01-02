#include <iostream>

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

// Runs before main()
int runExtra() {
    ExtraStruct e;
    std::cout << "ExtraStruct value: " << e.value << std::endl;
    extraFunction();
    return 0;
}

// Global initialization (executes before main)
int _ = runExtra();
