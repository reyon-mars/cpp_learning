#include <iostream>
#include <chrono>
#include <string>

class scoped_timer {
    
private:
    std::chrono::steady_clock::time_point start;
    std::string name;
    bool stopped = false;   // ✅ added

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
          name(std::move(other.name)),
          stopped(other.stopped)   // ✅ added
    {
        other.stopped = true;      // ✅ avoid double print
    }

    // Optional reset
    void reset() {
        start = std::chrono::steady_clock::now();
        stopped = false;           // ✅ added
    }

    // -------- NEW ADDITIONS --------

    // Manual stop
    void stop() {
        if (!stopped) {
            auto end = std::chrono::steady_clock::now();

            auto us = std::chrono::duration_cast<std::chrono::microseconds>(
                          end - start).count();

            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                          end - start).count();

            std::cout << name << ": "
                      << us << " us (" << ms << " ms)\n";

            stopped = true;
        }
    }

    // Get elapsed time without stopping
    long long elapsed_us() const {
        auto now = std::chrono::steady_clock::now();
        return std::chrono::duration_cast<std::chrono::microseconds>(
                   now - start).count();
    }

    // --------------------------------

    ~scoped_timer() {
        stop();   // ✅ replaced direct print with safe stop
    }
};

// -------- Helper function --------
template<typename Func>
void measure(const std::string& label, Func&& f) {
    auto start = std::chrono::steady_clock::now();
    f();
    auto end = std::chrono::steady_clock::now();

    std::cout << label << ": "
              << std::chrono::duration_cast<std::chrono::microseconds>(
                     end - start).count()
              << " us\n";
}
// --------------------------------


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

    // -------- NEW USAGE --------

    {
        scoped_timer timer("Manual stop demo");
        for (volatile int i = 0; i < 500000; ++i);
        std::cout << "Mid elapsed: "
                  << timer.elapsed_us() << " us\n";
        timer.stop();   // stops early
    }

    measure("Quick measure", [] {
        for (volatile int i = 0; i < 300000; ++i);
    });

    // --------------------------------

    return 0;
}
