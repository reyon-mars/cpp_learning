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

    void print() const {
        std::cout << "Counter value: " << value << std::endl;
    }

    // NEW: decrement helper
    int decrement(int amount) {
        return std::exchange(value, value - amount);
    }

    // NEW: add multiple values
    void add_multiple(int times, int amount) {
        for (int i = 0; i < times; ++i) {
            value += amount;
        }
    }

    // NEW: check if positive
    bool is_positive() const {
        return value > 0;
    }
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

    // Additional small tests
    countr.increment(10);
    countr.print();

    countr.reset();
    countr.print();

    // ---------------- NEW SMALL TESTS ----------------

    countr.increment(20);
    countr.decrement(5);
    countr.print();

    countr.add_multiple(3, 2); // adds 2 three times
    countr.print();

    std::cout << "Is positive? "
              << (countr.is_positive() ? "Yes" : "No") << std::endl;

    // -----------------------------------------------

    return 0;
}
