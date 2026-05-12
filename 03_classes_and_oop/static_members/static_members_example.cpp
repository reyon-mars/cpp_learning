#include <iostream>
#include <vector>
#include <memory>   // ✅ ADDED
#include <utility>  // ✅ ADDED
#include <cassert>  // ✅ ADDED

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

        std::cout << "Constructed ID: "
                  << id << "\n";
    }

    Counter(const Counter&) : id(next_id++) {
        total_created++;
        currently_alive++;

        std::cout << "Counter copied (new ID: "
                  << id << ")\n";
    }

    // ✅ ADDED: move constructor
    Counter(Counter&& other) noexcept
        : id(next_id++) {

        total_created++;
        currently_alive++;

        std::cout << "Counter moved (new ID: "
                  << id << ")\n";
    }

    // -------- EXTRA ADDITION --------
    Counter& operator=(const Counter&) {
        std::cout << "Copy assignment\n";
        return *this;
    }

    Counter& operator=(Counter&&) noexcept {
        std::cout << "Move assignment\n";
        return *this;
    }
    // --------------------------------

    ~Counter() {
        currently_alive--;

        std::cout << "Destroyed ID: "
                  << id << "\n";
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
        std::cout << "Object address: "
                  << this
                  << " (ID: "
                  << id
                  << ")\n";
    }

    // ✅ ADDED: show ID
    int getId() const {
        return id;
    }

    // ✅ ADDED: static helper
    static int getNextId() {
        return next_id;
    }

    // -------- EXTRA ADDITION --------
    void printInfo() const {
        std::cout << "[Counter ID: "
                  << id
                  << "] alive objects = "
                  << currently_alive
                  << "\n";
    }
    // --------------------------------
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


// ----------- EXTRA ADDITIONS -----------

// Pass by value (copy/move demo)
void passByValue(Counter c) {
    std::cout << "Inside passByValue()\n";
    c.printInfo();
}

// Return object
Counter createCounter() {
    Counter temp;
    return temp;
}

// Shared pointer demo
void sharedPointerDemo() {
    std::cout << "\n---- Shared Pointer Demo ----\n";

    std::shared_ptr<Counter> sp1 =
        std::make_shared<Counter>();

    std::cout << "use_count = "
              << sp1.use_count() << "\n";

    {
        std::shared_ptr<Counter> sp2 = sp1;

        std::cout << "use_count = "
                  << sp1.use_count() << "\n";
    }

    std::cout << "After inner scope use_count = "
              << sp1.use_count() << "\n";
}

// Weak pointer demo
void weakPointerDemo() {
    std::cout << "\n---- Weak Pointer Demo ----\n";

    std::shared_ptr<Counter> sp =
        std::make_shared<Counter>();

    std::weak_ptr<Counter> wp = sp;

    std::cout << "Expired? "
              << (wp.expired() ? "Yes" : "No")
              << "\n";

    sp.reset();

    std::cout << "Expired after reset? "
              << (wp.expired() ? "Yes" : "No")
              << "\n";
}

// --------------------------------------


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

    assert(c6 != nullptr); // ✅ ADDED

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

    std::unique_ptr<Counter> smart =
        std::make_unique<Counter>(); // ✅ ADDED

    Counter::printStats();

    std::cout << "\nNext ID will be: "
              << Counter::getNextId() << "\n";

    // -------- EXTRA FEATURE USAGE --------

    std::cout << "\n---- Pass By Value ----\n";

    passByValue(c1);

    std::cout << "\n---- Return Object Demo ----\n";

    Counter returned = createCounter();

    returned.printInfo();

    std::cout << "\n---- Move Semantics Demo ----\n";

    Counter moved = std::move(c2);

    moved.printInfo();

    std::cout << "\n---- Vector push_back Demo ----\n";

    std::vector<Counter> moreVec;

    moreVec.push_back(Counter());

    Counter extra;
    moreVec.push_back(extra);

    std::cout << "\n---- Shared Ownership ----\n";

    sharedPointerDemo();

    std::cout << "\n---- Weak Pointer ----\n";

    weakPointerDemo();

    std::cout << "\n---- Individual IDs ----\n";

    std::cout << "c1 ID: " << c1.getId() << "\n";
    std::cout << "c3 ID: " << c3.getId() << "\n";
    std::cout << "moved ID: " << moved.getId() << "\n";

    // -------- RESET DEMO --------

    std::cout << "\n---- Reset stats (demo) ----\n";

    Counter::resetStats();

    Counter::printStats();

    // ----------------------------------

    return 0;
}
