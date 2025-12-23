#include <string>
#include <string_view>
#include <iostream>
#include "scoped_timer.hpp"

// ======================================================
// ORIGINAL CODE
// ======================================================

void log_msg(std::string_view tag, std::string_view msg)
{
    std::cout << '[' << tag << ']' << msg << '\n';
}

void log_msg_(std::string tag, std::string msg)
{
    std::cout << '[' << tag << ']' << msg << '\n';
}

int main(void)
{
    const int iterations = 1000000;
    std::string test_tag = "Tag";
    std::string test_msg = "Message";

    // Benchmark log_msg (std::string_view)
    {
        scoped_timer timer_sv("String View (Total)");
        for (int i = 0; i < iterations; ++i) {
            log_msg(test_tag, test_msg);
        }
    }

    std::cout << "\n";

    // Benchmark log_msg_ (std::string copy)
    {
        scoped_timer timer_str("Normal String (Total)");
        for (int i = 0; i < iterations; ++i) {
            log_msg_(test_tag, test_msg);
        }
    }

    return 0;
}

// ======================================================
// EXTRA CODE (append only)
// ======================================================

// Fast logger using C-style strings
void log_msg_fast(const char* tag, const char* msg)
{
    std::cout << '[' << tag << ']' << msg << '\n';
}

// Benchmark for fast logger
void test_fast_logger()
{
    scoped_timer timer_fast("C-string Logger (Total)");

    for (int i = 0; i < 500000; ++i) {
        log_msg_fast("FAST", "Logging quickly!");
    }
}

// Auto-run benchmark before main()
struct LoggerBenchmark
{
    LoggerBenchmark()
    {
        std::cout << "\n=== Running Extra Benchmark ===\n";
        test_fast_logger();
    }
};

// Global object triggers constructor before main()
LoggerBenchmark __auto_logger_bench;
