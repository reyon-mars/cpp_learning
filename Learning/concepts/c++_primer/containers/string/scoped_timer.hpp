#include <iostream>
#include <chrono>
#include <string>

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

        std::cout << name << ": "
                  << us << " us"
                  << " (" << ms << " ms)"
                  << "\n";
    }
};

// Simple workload
void busy_work(int n) {
    volatile int x = 0; // prevents optimization
    for (int i = 0; i < n; ++i)
        x += i;
}

// ======================================================
// SMALL ADDITION ONLY
// ======================================================

// Reusable helper to measure a workload
void timed_work(const std::string& label, int n) {
    scoped_timer t(label);
    busy_work(n);
}

// Very small extra helper
void counted_work(const std::string& label, int n, int& counter) {
    scoped_timer t(label);
    busy_work(n);
    ++counter;
}

// ---------------------------------------------------------
// MAIN
// ---------------------------------------------------------

int main() {
    std::cout << "--- Scoped Timer Demo ---\n";

    // ---- total runtime timer ----
    scoped_timer total("total program time");

    scoped_timer t("main workload");
    busy_work(300'000);

    // ---- small added scope ----
    {
        scoped_timer inner("inner block workload");
        busy_work(150'000);
    }
    // ---------------------------

    // ---- very small extra usage ----
    timed_work("helper function workload", 200'000);
    // -------------------------------

    // ---- tiny added logic ----
    int workCount = 0;
    counted_work("counted workload 1", 100'000, workCount);
    counted_work("counted workload 2", 120'000, workCount);

    std::cout << "Work functions executed: "
              << workCount << std::endl;
    // --------------------------

    std::cout << "--- Program Finished ---\n";
    return 0;
}
