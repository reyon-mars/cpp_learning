#include <iostream>
#include <vector>
#include <memory>
#include <utility>
#include <cassert>

class Counter {
public:
    Counter() : id_{next_id_++} {
        ++total_created_;
        ++currently_alive_;
        std::cout << "Constructed  id=" << id_ << "\n";
    }

    Counter(const Counter&) : id_{next_id_++} {
        ++total_created_;
        ++currently_alive_;
        std::cout << "Copied       id=" << id_ << "\n";
    }

    Counter(Counter&&) noexcept : id_{next_id_++} {
        ++total_created_;
        ++currently_alive_;
        std::cout << "Moved        id=" << id_ << "\n";
    }

    Counter& operator=(const Counter&) noexcept {
        std::cout << "Copy-assigned id=" << id_ << "\n";
        return *this;
    }

    Counter& operator=(Counter&&) noexcept {
        std::cout << "Move-assigned id=" << id_ << "\n";
        return *this;
    }

    ~Counter() {
        --currently_alive_;
        std::cout << "Destroyed    id=" << id_ << "\n";
    }

    [[nodiscard]] int getId() const noexcept { return id_; }

    void printAddress() const {
        std::cout << "Address=" << static_cast<const void*>(this)
                  << " id=" << id_ << "\n";
    }

    void printInfo() const {
        std::cout << "[id=" << id_
                  << " alive=" << currently_alive_ << "]\n";
    }

    static void        printStats()   {
        std::cout << "total_created=" << total_created_
                  << " currently_alive=" << currently_alive_ << "\n";
    }

    static void resetStats() noexcept {
        total_created_   = 0;
        currently_alive_ = 0;
        next_id_         = 0;
        std::cout << "Stats reset\n";
    }

    [[nodiscard]] static int getTotalCreated()   noexcept { return total_created_;   }
    [[nodiscard]] static int getCurrentlyAlive() noexcept { return currently_alive_; }
    [[nodiscard]] static int getNextId()         noexcept { return next_id_;         }

private:
    int id_;

    static inline int total_created_   = 0;
    static inline int currently_alive_ = 0;
    static inline int next_id_         = 0;
};

class ScopeTracker {
public:
    ScopeTracker()  { std::cout << "[Entering scope]\n"; }
    ~ScopeTracker() { std::cout << "[Exiting scope]\n"; Counter::printStats(); }
};

void passByValue(Counter c) {
    std::cout << "passByValue: ";
    c.printInfo();
}

[[nodiscard]] Counter createCounter() {
    return Counter{};
}

void sharedPointerDemo() {
    std::cout << "\n--- Shared pointer ---\n";
    auto sp1 = std::make_shared<Counter>();
    std::cout << "use_count=" << sp1.use_count() << "\n";
    {
        auto sp2 = sp1;
        std::cout << "use_count=" << sp1.use_count() << "\n";
    }
    std::cout << "use_count after inner scope=" << sp1.use_count() << "\n";
}

void weakPointerDemo() {
    std::cout << "\n--- Weak pointer ---\n";
    auto sp = std::make_shared<Counter>();
    std::weak_ptr<Counter> wp = sp;
    std::cout << "expired=" << std::boolalpha << wp.expired() << "\n";
    sp.reset();
    std::cout << "expired after reset=" << wp.expired() << "\n";
}

int main() {
    std::cout << "--- Construction ---\n";
    Counter c1, c2, c3;
    Counter::printStats();

    std::cout << "\n--- Scoped block ---\n";
    {
        ScopeTracker tracker;
        Counter c4;
        Counter::printStats();
    }

    std::cout << "\n--- After block ---\n";
    Counter::printStats();

    std::cout << "\n--- Copy ---\n";
    Counter c5 = c1;
    Counter::printStats();

    std::cout << "\n--- Dynamic allocation ---\n";
    {
        auto c6 = std::make_unique<Counter>();
        assert(c6 != nullptr);
        Counter::printStats();
    }
    std::cout << "After unique_ptr scope:\n";
    Counter::printStats();

    std::cout << "\n--- Vector (default construction) ---\n";
    std::vector<Counter> vec(3);
    Counter::printStats();

    std::cout << "\n--- Addresses ---\n";
    c1.printAddress();
    c2.printAddress();

    std::cout << "\n--- unique_ptr ---\n";
    auto smart = std::make_unique<Counter>();
    Counter::printStats();
    std::cout << "Next id will be=" << Counter::getNextId() << "\n";

    std::cout << "\n--- Pass by value ---\n";
    passByValue(c1);

    std::cout << "\n--- Return object (NRVO) ---\n";
    Counter returned = createCounter();
    returned.printInfo();

    std::cout << "\n--- Move ---\n";
    Counter moved = std::move(c2);
    moved.printInfo();

    std::cout << "\n--- vector::push_back ---\n";
    std::vector<Counter> moreVec;
    moreVec.reserve(2);
    moreVec.push_back(Counter{});
    Counter extra;
    moreVec.push_back(extra);

    sharedPointerDemo();
    weakPointerDemo();

    std::cout << "\n--- IDs ---\n";
    std::cout << "c1=" << c1.getId()
              << " c3=" << c3.getId()
              << " moved=" << moved.getId() << "\n";

    std::cout << "\n--- Reset ---\n";
    Counter::resetStats();
    Counter::printStats();

    return 0;
}
