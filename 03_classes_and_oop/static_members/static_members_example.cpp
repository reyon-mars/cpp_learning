// Static Members Exercise
// Class variables and member functions

#include <iostream>

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

    Counter(const Counter&) {   // 🔹 added (copy constructor)
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

    std::cout << "\n---- Copy test ----\n";   // 🔹 added
    Counter c5 = c1;   // copy constructor called
    Counter::printStats();

    std::cout << "\n---- Dynamic allocation ----\n";   // 🔹 added
    Counter* c6 = new Counter();
    Counter::printStats();
    delete c6;

    std::cout << "\nAfter delete:\n";   // 🔹 added
    Counter::printStats();

    return 0;
}
