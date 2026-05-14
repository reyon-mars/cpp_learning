#include <iostream>
#include <chrono>
#include <string>
#include <thread>
#include <utility>
#include <functional>   // ✅ ADDED (for callable utilities safety)
#include <vector>       // ✅ ADDED
#include <numeric>      // ✅ ADDED

class scoped_timer {
    
private:
    std::chrono::steady_clock::time_point start;
    std::string name;
    bool stopped = false;

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
          stopped(other.stopped)
    {
        other.stopped = true;
    }

    // -------- EXTRA ADDITION --------
    scoped_timer& operator=(scoped_timer&& other) noexcept {
        if (this != &other) {
            start = other.start;
            name = std::move(other.name);
            stopped = other.stopped;
            other.stopped = true;
        }
        return *this;
    }
    // --------------------------------

    // Manual stop
    void stop() {
        if (!stopped) {
            auto end = std::chrono::steady_clock::now();

            auto duration_us =
                std::chrono::duration_cast<std::chrono::microseconds>(
                    end - start).count();

            auto duration_ms =
                std::chrono::duration_cast<std::chrono::milliseconds>(
                    end - start).count();

            std::cout << name << ": "
                      << duration_us << " us ("
                      << duration_ms << " ms)\n";

            stopped = true;
        }
    }

    // -------- NEW ADDITIONS --------

    // Get elapsed time without stopping
    long long elapsed_us() const {
        auto now = std::chrono::steady_clock::now();
        return std::chrono::duration_cast<std::chrono::microseconds>(
                   now - start).count();
    }

    // Restart timer
    void restart() {
        start = std::chrono::steady_clock::now();
        stopped = false;
    }

    // Static helper for quick measurement
    template<typename Func>
    static void measure(const std::string& label, Func&& f) {
        auto begin = std::chrono::steady_clock::now();
        f();
        auto end = std::chrono::steady_clock::now();

        auto duration =
            std::chrono::duration_cast<std::chrono::microseconds>(
                end - begin).count();

        std::cout << label << ": " << duration << " us\n";
    }

    // -------- EXTRA SMALL ADDITIONS --------

    // Get elapsed in milliseconds
    long long elapsed_ms() const {
        auto now = std::chrono::steady_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(
                   now - start).count();
    }

    // Check if timer already stopped
    bool is_stopped() const {
        return stopped;
    }

    // Measure and return duration (instead of printing)
    template<typename Func>
    static long long measure_return(Func&& f) {
        auto begin = std::chrono::steady_clock::now();
        f();
        auto end = std::chrono::steady_clock::now();

        return std::chrono::duration_cast<std::chrono::microseconds>(
                   end - begin).count();
    }

    // -------- EXTRA ADDITIONS --------

    // Print elapsed without stopping
    void print_elapsed() const {
        std::cout << "[Elapsed] "
                  << elapsed_us() << " us\n";
    }

    // Sleep helper
    static void sleep_ms(int ms) {
        std::this_thread::sleep_for(
            std::chrono::milliseconds(ms));
    }

    // --------------------------------

    ~scoped_timer() {
        stop();
    }
};

// -------- EXTRA HELPER FUNCTION --------
void heavy_task() {
    std::vector<int> values(100000, 1);

    volatile int sum =
        std::accumulate(values.begin(),
                        values.end(), 0);

    (void)sum;
}
// ---------------------------------------

// ---------------- Example Usage ----------------

int main() {

    {
        scoped_timer timer("Sleep 100ms");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    {
        scoped_timer timer("Manual stop example");
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        timer.stop();
    }

    // -------- NEW USAGE --------

    {
        scoped_timer timer("Elapsed check");
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
        std::cout << "Elapsed (mid): "
                  << timer.elapsed_us() << " us\n";
    }

    {
        scoped_timer timer("Restart demo");
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        timer.restart();
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }

    // Quick measurement without object
    scoped_timer::measure("Lambda task", [] {
        std::this_thread::sleep_for(std::chrono::milliseconds(40));
    });

    // -------- EXTRA USAGE --------

    {
        scoped_timer timer("Check stopped flag");
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        std::cout << "Is stopped? " << timer.is_stopped() << "\n";
    }

    auto duration = scoped_timer::measure_return([] {
        std::this_thread::sleep_for(std::chrono::milliseconds(25));
    });
    std::cout << "Measured (returned): " << duration << " us\n";

    // -------- NEW EXTRA USAGE --------

    {
        scoped_timer timer("Print elapsed demo");
        scoped_timer::sleep_ms(15);
        timer.print_elapsed();
    }

    std::cout << "\n--- Heavy Task Timing ---\n";
    scoped_timer::measure("Vector accumulate", heavy_task);

    std::cout << "\n--- Move Assignment Demo ---\n";
    scoped_timer t1("Timer A");
    scoped_timer::sleep_ms(5);

    scoped_timer t2("Timer B");
    t2 = std::move(t1);

    scoped_timer::sleep_ms(5);

    // --------------------------------

    return 0;
}
