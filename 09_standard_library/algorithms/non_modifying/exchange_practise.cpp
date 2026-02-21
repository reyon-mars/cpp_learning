#include <iostream>
#include <utility>

// ======================================================
// ORIGINAL CLASS (LOGIC UNCHANGED)
// ======================================================

class counter {
private:
    int value = 0;

public:
    int increment(int amount) {
        return std::exchange(value, value + amount);
    }

    int reset() {
        return std::exchange(value, 0);
    }

    // ---- small added helpers ----
    int current() const { return value; }
    bool is_zero() const { return value == 0; }
    // -----------------------------
};

// ======================================================
// MAIN
// ======================================================

int main(void) {

    counter countr;

    for (int i = 1; i < 100; i++) {
        std::cout << countr.increment(i) << " ";
    }
    std::cout << countr.reset() << std::endl;

    // ---- very small added code ----
    std::cout << "Current value: "
              << countr.current() << std::endl;

    std::cout << "Is counter zero? "
              << (countr.is_zero() ? "Yes" : "No") << std::endl;
    // -------------------------------

    return 0;
}
