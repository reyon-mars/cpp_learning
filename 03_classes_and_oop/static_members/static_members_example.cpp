// Static Members Exercise
// Class variables and member functions

#include <iostream>

class Counter {
private:
    int instance_count = 0;   // still per-object (unused but kept)
    static int total_created;
    static int currently_alive;   // 🔹 added

public:
    Counter() {
        total_created++;
        currently_alive++;        // 🔹 added
    }

    ~Counter() {                  // 🔹 added
        currently_alive--;
    }

    static int getTotalCreated() {
        return total_created;
    }

    static int getCurrentlyAlive() {   // 🔹 added
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
int Counter::currently_alive = 0;   // 🔹 added

int main() {

    Counter c1, c2, c3;

    std::cout << "Total created: "
              << Counter::getTotalCreated() << "\n";

    Counter::printStats();

    std::cout << "Access via object: "
              << c1.getTotalCreated() << "\n";  // 🔹 allowed but not recommended

    {
        Counter c4;   // 🔹 scoped object
        std::cout << "\nInside block:\n";
        Counter::printStats();
    } // c4 destroyed here

    std::cout << "\nAfter block:\n";
    Counter::printStats();

    return 0;
}
