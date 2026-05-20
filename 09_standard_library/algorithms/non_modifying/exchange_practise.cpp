#include <iostream>
#include <utility>
#include <vector>      // 🔹 ADDED
#include <numeric>     // 🔹 ADDED
#include <algorithm>   // 🔹 ADDED

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

    // 🔹 NEW: set value directly
    void set(int v) {
        value = v;
    }

    // 🔹 NEW: multiply value
    void multiply(int factor) {
        value *= factor;
    }

    // 🔹 NEW: simple add wrapper
    void add(int amount) {
        value += amount;
    }

    // ==================================================
    // 🔥 EXTRA SMALL ADDITIONS
    // ==================================================

    // 🔹 NEW: divide safely
    bool divide(int divisor) {
        if (divisor == 0) {
            return false;
        }

        value /= divisor;
        return true;
    }

    // 🔹 NEW: absolute value
    void make_positive() {
        value = std::abs(value);
    }

    // 🔹 NEW: compare values
    bool equals(int other) const {
        return value == other;
    }

    // 🔹 NEW: increment repeatedly
    void increment_loop(int times) {
        for (int i = 0; i < times; ++i) {
            ++value;
        }
    }

    // 🔹 NEW: maximum assignment
    void max_with(int other) {
        value = std::max(value, other);
    }

    // --------------------------------------------------
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
              << (countr.is_zero() ? "Yes" : "No")
              << std::endl;

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
              << (countr.is_positive() ? "Yes" : "No")
              << std::endl;

    // 🔹 NEW TESTS

    countr.set(10);
    countr.print();

    countr.multiply(3);
    countr.print();

    countr.add(7);
    countr.print();

    // ==================================================
    // 🔥 EXTRA TESTS
    // ==================================================

    std::cout << "\n--- Extra Counter Operations ---\n";

    // divide demo
    if (countr.divide(2)) {
        std::cout << "Division successful\n";
    }

    countr.print();

    // divide by zero protection
    if (!countr.divide(0)) {
        std::cout << "Division by zero prevented\n";
    }

    // negative to positive
    countr.set(-50);
    countr.print();

    countr.make_positive();
    countr.print();

    // equality check
    std::cout << "Equals 50? "
              << (countr.equals(50) ? "Yes" : "No")
              << std::endl;

    // repeated increment
    countr.increment_loop(5);
    countr.print();

    // max comparison
    countr.max_with(100);
    countr.print();

    // 🔹 vector utility demo
    std::vector<int> nums = {1, 2, 3, 4, 5};

    int total = std::accumulate(
        nums.begin(),
        nums.end(),
        0
    );

    std::cout << "Accumulated vector total: "
              << total << std::endl;

    // 🔹 algorithm demo
    auto max_it = std::max_element(
        nums.begin(),
        nums.end()
    );

    std::cout << "Largest vector element: "
              << *max_it << std::endl;

    // -------------------------------------------------

    return 0;
}
