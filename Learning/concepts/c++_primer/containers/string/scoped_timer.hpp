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
        auto sec = std::chrono::duration_cast<std::chrono::seconds>(end - start).count(); // NEW

        std::cout << name << ": "
                  << us << " us"
                  << " (" << ms << " ms"
                  << ", " << sec << " s)"
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

// ---------------------------------------------------------
// MAIN
// ---------------------------------------------------------

int main() {
    std::cout << "--- Scoped Timer Demo ---\n";

    print_status("Program started");

    int timerRuns = 0; // NEW tiny counter

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

    std::cout << "Work functions executed: " << workCount << std::endl;
    std::cout << "Total timed sections: " << timerRuns << std::endl; // NEW

    print_status("Program ending");
    std::cout << "--- Program Finished ---\n";

    return 0;
}
