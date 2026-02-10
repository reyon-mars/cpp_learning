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

// ===== VERY SMALL EXTRA HELPERS =====

// Run same work multiple times
void repeat_work(const std::string& label, int n, int times) {
    scoped_timer t(label);
    for (int i = 0; i < times; ++i)
        busy_work(n);
}

// Average style runner (simple demo only)
void average_work_demo(const std::string& label, int n, int runs) {
    scoped_timer t(label);
    for (int i = 0; i < runs; ++i)
        busy_work(n);
}

// Simple status print
void print_status(const std::string& msg) {
    std::cout << "[Status] " << msg << std::endl;
}

// ====================================

// ---------------------------------------------------------
// MAIN
// ---------------------------------------------------------

int main() {
    std::cout << "--- Scoped Timer Demo ---\n";

    print_status("Program started");

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

    // ---- NEW VERY SMALL EXTRAS ----
    repeat_work("repeat workload x3", 80'000, 3);
    average_work_demo("average demo workload", 60'000, 5);
    // ------------------------------

    print_status("Program ending");

    std::cout << "--- Program Finished ---\n";
    return 0;
}
 
