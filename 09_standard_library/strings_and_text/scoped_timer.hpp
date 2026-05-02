#include <iostream>
#include <chrono>
#include <string>
#include <thread>
#include <ctime>
#include <numeric>   // added
#include <vector>    // tiny addition

// ======================================================
// ORIGINAL CODE (UNCHANGED LOGIC)
// ======================================================

class scoped_timer {

private:
    std::chrono::steady_clock::time_point start;
    std::string name;

public:
    explicit scoped_timer(std::string str)
        : start(std::chrono::steady_clock::now()),
          name(std::move(str)) {}

    ~scoped_timer() {
        auto end = std::chrono::steady_clock::now();

        auto us = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        auto sec = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();

        std::cout << name << ": "
                  << us << " us"
                  << " (" << ms << " ms"
                  << ", " << sec << " s)"
                  << "\n";
    }
};

// Simple workload
void busy_work(int n) {
    volatile int x = 0;
    for (int i = 0; i < n; ++i)
        x += i;
}

// ======================================================
// SMALL ADDITION ONLY
// ======================================================

void timed_work(const std::string& label, int n) {
    scoped_timer t(label);
    busy_work(n);
}

void counted_work(const std::string& label, int n, int& counter) {
    scoped_timer t(label);
    busy_work(n);
    ++counter;
}

// ===== VERY SMALL EXTRA HELPERS =====

void repeat_work(const std::string& label, int n, int times) {
    scoped_timer t(label);
    for (int i = 0; i < times; ++i)
        busy_work(n);
}

void print_status(const std::string& msg) {
    std::cout << "[Status] " << msg << std::endl;
}

// --- Small new helper: simulate delay ---
void timed_sleep(const std::string& label, int ms) {
    scoped_timer t(label);
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

// --- Small new helper: print current time ---
void print_current_time() {
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::cout << "[Current time] " << std::ctime(&now);
}

// ---------------------------------------------------------
// MAIN
// ---------------------------------------------------------

int main() {
    std::cout << "--- Scoped Timer Demo ---\n";

    print_status("Program started");
    print_current_time();

    int timerRuns = 0;

    scoped_timer total("total program time"); ++timerRuns;

    {
        scoped_timer t("main workload"); ++timerRuns;
        busy_work(300'000);
    }

    {
        scoped_timer inner("inner block workload"); ++timerRuns;
        busy_work(150'000);
    }

    timed_work("helper function workload", 200'000); ++timerRuns;

    int workCount = 0;
    counted_work("counted workload 1", 100'000, workCount); ++timerRuns;
    counted_work("counted workload 2", 120'000, workCount); ++timerRuns;

    repeat_work("repeat workload x3", 80'000, 3); ++timerRuns;

    // small demo of timed sleep
    timed_sleep("sleep test (200ms)", 200); ++timerRuns;

    std::cout << "Work functions executed: " << workCount << std::endl;
    std::cout << "Total timed sections: " << timerRuns << std::endl;

    print_status("Program ending");
    std::cout << "--- Program Finished ---\n";

    // ===== EXTRA SMALL ADDITIONS (NEW) =====

    // Measure quick inline block
    {
        scoped_timer t("quick inline block");
        busy_work(50'000);
    }

    // Average time simulation (manual)
    std::vector<long long> sample_times = {100, 200, 150, 175};
    long long total_time = std::accumulate(sample_times.begin(), sample_times.end(), 0LL);
    std::cout << "Average sample time: "
              << (total_time / sample_times.size()) << " us\n";

    // Check duration type
    auto now = std::chrono::steady_clock::now();
    std::cout << "Clock is steady? "
              << (std::chrono::steady_clock::is_steady ? "Yes" : "No") << "\n";

    // ===== FINAL TINY ADDITIONS =====

    // Run multiple timed blocks in loop
    for (int i = 1; i <= 3; ++i) {
        scoped_timer t("loop workload " + std::to_string(i));
        busy_work(30'000 * i);
    }

    // Compare two workloads
    {
        scoped_timer t("comparison workload A");
        busy_work(60'000);
    }
    {
        scoped_timer t("comparison workload B");
        busy_work(120'000);
    }

    // Simple throughput estimate
    int ops = 100000;
    auto start = std::chrono::steady_clock::now();
    busy_work(ops);
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << "Throughput: "
              << ops / (duration ? duration : 1)
              << " ops/us\n";

    // ======================================

    return 0;
}
