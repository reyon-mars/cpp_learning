#include <iostream>
#include <chrono>
#include <string>

class scoped_timer {
    
private:
    std::chrono::steady_clock::time_point start;
    std::string name;

public:
    explicit scoped_timer(std::string str)
        : start(std::chrono::steady_clock::now()),
          name(std::move(str)) {}

    // Prevent copying
    scoped_timer(const scoped_timer&) = delete;
    scoped_timer& operator=(const scoped_timer&) = delete;

    // Allow moving
    scoped_timer(scoped_timer&& other) noexcept
        : start(other.start),
          name(std::move(other.name)) {}

    // Optional reset
    void reset() {
        start = std::chrono::steady_clock::now();
    }

    ~scoped_timer() {
        auto end = std::chrono::steady_clock::now();
        std::cout << name << ": "
                  << std::chrono::duration_cast<std::chrono::microseconds>(
                         end - start).count()
                  << " us\n";
    }
};


// ---------------- Example Usage ----------------
int main() {

    {
        scoped_timer timer("Block 1");
        for (volatile int i = 0; i < 1000000; ++i);
    }

    {
        scoped_timer timer("Block 2");
        for (volatile int i = 0; i < 2000000; ++i);
    }

    return 0;
}
