#include <iostream>
#include <utility>

// ======================================================
// ORIGINAL CLASS (UNCHANGED LOGIC)
// ======================================================

class counter {
private:
    int value = 0;   // safe initialization

public:
    int increment(int amount) {
        return std::exchange(value, value + amount);
    }

    int reset() {
        return std::exchange(value, 0);
    }
};

// ======================================================
// SMALL EXTRA CODE (ADDED)
// ======================================================

// Simple helper function to demonstrate counter usage
void demo_counter() {
    std::cout << "\n[Extra Demo]\n";

    counter c;
    std::cout << "Increment by 5, previous value = "
              << c.increment(5) << std::endl;

    std::cout << "Increment by 3, previous value = "
              << c.increment(3) << std::endl;

    std::cout << "Reset, previous value = "
              << c.reset() << std::endl;
}

// ======================================================
// MAIN
// ======================================================

int main(void) {

    // ---------- ORIGINAL BEHAVIOR ----------
    counter countr;
    for (int i = 1; i < 100; i++) {
        std::cout << countr.increment(i) << " ";
    }
    std::cout << countr.reset() << std::endl;

    // ---------- SMALL EXTRA DEMO ----------
    demo_counter();

    return 0;
}
