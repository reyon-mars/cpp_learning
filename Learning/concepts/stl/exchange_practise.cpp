#include <iostream>
#include <utility>

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

// ----------------------------------------------------
// EXTRA CODE ADDED BELOW (append only)
// ----------------------------------------------------

// Helper to test counter behavior
void test_counter(counter& c) {
    std::cout << "\n[Extra] Testing counter\n";

    std::cout << "Increment by 5, previous = "
              << c.increment(5) << '\n';

    std::cout << "Increment by 10, previous = "
              << c.increment(10) << '\n';

    std::cout << "Reset, previous = "
              << c.reset() << '\n';
}

// Demonstrates multiple operations safely
void run_extra_counter_tests() {
    counter temp;

    test_counter(temp);

    std::cout << "Reset again, previous = "
              << temp.reset() << '\n';
}

// ----------------------------------------------------
// MAIN (original logic preserved + explicit call)
// ----------------------------------------------------

int main(void) {

    counter countr;
    for (int i = 1; i < 100; i++) {
        std::cout << countr.increment(i) << " ";
    }
    std::cout << countr.reset() << '\n';

    // ---- extra code call ----
    run_extra_counter_tests();

    return 0;
}
