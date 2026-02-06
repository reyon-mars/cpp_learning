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

    // -------- small additions --------
    int current() const {
        return value;
    }

    bool is_zero() const {
        return value == 0;
    }

    int decrement(int amount) {
        return std::exchange(value, value - amount);
    }

    bool is_positive() const {
        return value > 0;
    }
    // --------------------------------
};

// ======================================================
// VERY SMALL EXTRA CODE (ADDED)
// ======================================================

// Stream output helper
std::ostream& operator<<(std::ostream& os, const counter& c) {
    return os << c.current();
}

// Simple helper function to demonstrate counter usage
void demo_counter() {
    std::cout << "\n[Extra Demo]\n";

    counter c;
    std::cout << "Increment by 5, previous value = "
              << c.increment(5) << std::endl;

    std::cout << "Increment by 3, previous value = "
              << c.increment(3) << std::endl;

    std::cout << "Decrement by 4, previous value = "
              << c.decrement(4) << std::endl;

    std::cout << "Current value = "
              << c.current() << std::endl;

    std::cout << "Reset, previous value = "
              << c.reset() << std::endl;

    std::cout << "Is counter zero? "
              << (c.is_zero() ? "Yes" : "No") << std::endl;
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

    // ---- small added usage ----
    std::cout << "Counter after reset = "
              << countr.current() << std::endl;

    std::cout << "Printed via operator<< : "
              << countr << std::endl;

    std::cout << "Is counter positive? "
              << (countr.is_positive() ? "Yes" : "No") << std::endl;

    // ---------- SMALL EXTRA DEMO ----------
    demo_counter();

    return 0;
}
