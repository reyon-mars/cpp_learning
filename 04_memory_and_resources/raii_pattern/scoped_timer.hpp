#include <iostream>
#include <chrono>
#include <string>
#include <utility>     // ✅ ADDED (for std::move)
#include <functional>  // ✅ ADDED (for callable templates)
#include <thread>      // ✅ ADDED
#include <vector>      // ✅ ADDED
#include <numeric>     // ✅ ADDED

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

    // ✅ ADDED: move assignment
    scoped_timer& operator=(scoped_timer&& other) noexcept {
        if (this != &other) {
            start = other.start;
            name = std::move(other.name);
            stopped = other.stopped;
            other.stopped = true;
        }
        return *this;
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

    // -------- EXTRA SMALL ADDITIONS --------

    // Get elapsed time in milliseconds
    long long elapsed_ms() const {
        auto now = std::chrono::steady_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(
                   now - start).count();
    }

    // Check if already stopped
    bool is_stopped() const {
        return stopped;
    }

    // ✅ ADDED: print elapsed without stopping
    void print_elapsed() const {
        std::cout << "[Elapsed] "
                  << elapsed_us() << " us\n";
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

// -------- EXTRA HELPER --------

// Returns measured time instead of printing
template<typename Func>
long long measure_return(Func&& f) {
    auto start = std::chrono::steady_clock::now();
    f();
    auto end = std::chrono::steady_clock::now();

    return std::chrono::duration_cast<std::chrono::microseconds>(
               end - start).count();
}

// ✅ ADDED: helper workload
void heavy_task() {
    std::vector<int> values(100000, 1);

    volatile int sum =
        std::accumulate(values.begin(),
                        values.end(), 0);

    (void)sum;
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

        timer.print_elapsed();

        timer.stop();   // stops early
    }

    measure("Quick measure", [] {
        for (volatile int i = 0; i < 300000; ++i);
    });

    // -------- EXTRA USAGE --------

    {
        scoped_timer timer("Check state");
        for (volatile int i = 0; i < 100000; ++i);

        std::cout << "Stopped? "
                  << timer.is_stopped() << "\n";
    }

    auto t = measure_return([] {
        for (volatile int i = 0; i < 150000; ++i);
    });

    std::cout << "Returned time: "
              << t << " us\n";

    // -------- NEW SMALL ADDITIONS --------

    {
        scoped_timer timer("Reset demo");

        std::this_thread::sleep_for(
            std::chrono::milliseconds(20));

        std::cout << "Before reset: "
                  << timer.elapsed_ms() << " ms\n";

        timer.reset();

        std::this_thread::sleep_for(
            std::chrono::milliseconds(10));

        std::cout << "After reset: "
                  << timer.elapsed_ms() << " ms\n";
    }

    std::cout << "\n--- Heavy Task Timing ---\n";

    measure("Vector accumulate", heavy_task);

    std::cout << "\n--- Move Assignment Demo ---\n";

    scoped_timer t1("Timer A");

    std::this_thread::sleep_for(
        std::chrono::milliseconds(5));

    scoped_timer t2("Timer B");

    t2 = std::move(t1);

    std::this_thread::sleep_for(
        std::chrono::milliseconds(5));

    // --------------------------------

    return 0;
}
