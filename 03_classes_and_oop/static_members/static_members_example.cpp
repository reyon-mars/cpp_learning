#include <iostream>
#include <vector>
#include <memory>   // ✅ ADDED

class Counter {
private:
    int instance_count = 0;   
    static int total_created;
    static int currently_alive;

    // ✅ ADDED: unique ID for each object
    int id;
    static int next_id;

public:
    Counter() : id(next_id++) {
        total_created++;
        currently_alive++;
        std::cout << "Constructed ID: " << id << "\n";
    }

    Counter(const Counter&) : id(next_id++) {
        total_created++;
        currently_alive++;
        std::cout << "Counter copied (new ID: " << id << ")\n";
    }

    // ✅ ADDED: move constructor
    Counter(Counter&& other) noexcept : id(next_id++) {
        total_created++;
        currently_alive++;
        std::cout << "Counter moved (new ID: " << id << ")\n";
    }

    ~Counter() {
        currently_alive--;
        std::cout << "Destroyed ID: " << id << "\n";
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

    static void resetStats() {
        total_created = 0;
        currently_alive = 0;
        next_id = 0;
        std::cout << "Stats reset\n";
    }

    void printAddress() const {
        std::cout << "Object address: " << this
                  << " (ID: " << id << ")\n";
    }

    // ✅ ADDED: show ID
    int getId() const { return id; }

    // ✅ ADDED: static helper
    static int getNextId() {
        return next_id;
    }
};

// Static definitions
int Counter::total_created = 0;
int Counter::currently_alive = 0;
int Counter::next_id = 0;


// ✅ ADDED: RAII scope tracker
class ScopeTracker {
public:
    ScopeTracker() {
        std::cout << "[Entering scope]\n";
    }
    ~ScopeTracker() {
        std::cout << "[Exiting scope]\n";
        Counter::printStats();
    }
};


int main() {

    Counter c1, c2, c3;

    std::cout << "Total created: "
              << Counter::getTotalCreated() << "\n";

    Counter::printStats();

    std::cout << "Access via object: "
              << c1.getTotalCreated() << "\n";

    {
        ScopeTracker tracker; // ✅ ADDED
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
    std::vector<Counter> vec(3);
    Counter::printStats();

    std::cout << "\nAddresses of objects:\n";
    c1.printAddress();
    c2.printAddress();

    std::cout << "\n---- Smart Pointer Demo ----\n";
    std::unique_ptr<Counter> smart = std::make_unique<Counter>(); // ✅ ADDED
    Counter::printStats();

    std::cout << "\nNext ID will be: "
              << Counter::getNextId() << "\n";

    std::cout << "\n---- Reset stats (demo) ----\n";
    Counter::resetStats();
    Counter::printStats();

    // ----------------------------------

    return 0;
}
