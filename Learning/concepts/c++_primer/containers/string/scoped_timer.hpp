#include <iostream>
#include <chrono>
#include <string>


class scoped_timer {
    
    private:
    std::chrono::steady_clock::time_point start;
    std::string name;
    
    public:
    explicit scoped_timer( std::string str )
        : start(std::chrono::steady_clock::now()),
          name( std::move(str )) {}

    ~scoped_timer(){
        auto end = std::chrono::steady_clock::now();
        std::cout << name << ':'
                  << std::chrono::duration_cast<std::chrono::microseconds>( end - start ).count()
                  << " us" << '\n';
    }
};

// ---------------------------------------------------------
// EXTRA CODE ADDED BELOW (original code untouched)
// ---------------------------------------------------------

void busy_work(int n) {
    volatile int x = 0;
    for (int i = 0; i < n; ++i)
        x += i;
}

// Demonstrates scoped lifetime timing
void timed_function() {
    scoped_timer t("busy_work(1e6)");
    busy_work(1'000'000);
}

// Automatically executed before main()
int __ = [](){
    std::cout << "--- Extra scoped_timer demo ---\n";

    {
        scoped_timer t("Block scope timer");
        busy_work(500'000);
    }

    timed_function();
    return 0;
}();
