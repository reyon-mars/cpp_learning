// Static Members Exercise
// Class variables and member functions

#include <iostream>
#include <vector>   // ✅ added

class Counter {
private:
    int instance_count = 0;   // still per-object (unused but kept)
    static int total_created;
    static int currently_alive;

public:
    Counter() {
        total_created++;
        currently_alive++;
    }

    Counter(const Counter&) {
        total_created++;
        currently_alive++;
        std::cout << "Counter copied\n";
    }

    ~Counter() {
        currently_alive--;
    }

    static int getTotalCreated() {
        return total_created;
    }

    static int getCurrentlyAlive() {
        return currently_alive;
    }

    static void printStats() {
        std::cout << "Total objects created: "
                  << total_created << "\n";
        std::cout << "Currently alive: "
                  << currently_alive << "\n";
    }

    // ----------- NEW ADDITIONS -----------

    // Static reset function
    static void resetStats() {
        total_created = 0;
        currently_alive = 0;
        std::cout << "Stats reset\n";
    }

    // Instance method showing object address
    void printAddress() const {
        std::cout << "Object address: " << this << "\n";
    }

    // ------------------------------------
};

// Define and initialize static members
int Counter::total_created = 0;
int Counter::currently_alive = 0;

int main() {

    Counter c1, c2, c3;

    std::cout << "Total created: "
              << Counter::getTotalCreated() << "\n";

    Counter::printStats();

    std::cout << "Access via object: "
              << c1.getTotalCreated() << "\n";

    {
        Counter c4;
        std::cout << "\nInside block:\n";
        Counter::printStats();
    }

    std::cout << "\nAfter block:\n";
    Counter::printStats();

    std::cout << "\n---- Copy test ----\n";
    Counter c5 = c1;
    Counter::printStats();

    std::cout << "\n---- Dynamic allocation ----\n";
    Counter* c6 = new Counter();
    Counter::printStats();
    delete c6;

    std::cout << "\nAfter delete:\n";
    Counter::printStats();

    // -------- NEW FEATURE USAGE --------

    std::cout << "\n---- Vector of Counters ----\n";
    std::vector<Counter> vec(3);  // creates 3 more objects
    Counter::printStats();

    std::cout << "\nAddresses of objects:\n";
    c1.printAddress();
    c2.printAddress();

    std::cout << "\n---- Reset stats (demo) ----\n";
    Counter::resetStats();
    Counter::printStats();

    // ----------------------------------

    return 0;
}
