#include <string>
#include <string_view>
#include <iostream>
#include <chrono>
#include <ctime>
#include <numeric>   // added
#include <vector>    // tiny addition
#include <algorithm> // tiny addition
#include "scoped_timer.hpp"

// ======================================================
// ORIGINAL CODE (UNCHANGED LOGIC)
// ======================================================

void log_msg(std::string_view tag, std::string_view msg)
{
    std::cout << '[' << tag << ']' << msg << '\n';
}

void log_msg_(std::string tag, std::string msg)
{
    std::cout << '[' << tag << ']' << msg << '\n';
}

// ======================================================
// SMALL ADDITION ONLY
// ======================================================

// inline logger to avoid function call overhead
inline void log_msg_inline(std::string_view tag, std::string_view msg)
{
    std::cout << '[' << tag << ']' << msg << '\n';
}

// very small helper
void print_header(const std::string& title)
{
    std::cout << "\n--- " << title << " ---\n";
}

// ===== VERY SMALL EXTRA HELPERS =====

constexpr std::string_view DEFAULT_TAG = "Default";

// toggle logging (compile-time)
constexpr bool ENABLE_LOGGING = true;

// conditional logger (tiny wrapper)
inline void log_if_enabled(std::string_view tag, std::string_view msg)
{
    if constexpr (ENABLE_LOGGING)
        log_msg(tag, msg);
}

// tiny counter printer
void print_counter(const std::string& label, int value)
{
    std::cout << label << ": " << value << '\n';
}

// warmup logger (helps realistic benchmarking)
void warmup_logger()
{
    log_msg(DEFAULT_TAG, "Warmup");
}

// force flush helper
void flush_output()
{
    std::cout << std::flush;
}

// ===== NEW VERY SMALL ADDITIONS =====

// print separator line
void print_separator()
{
    std::cout << "---------------------------------\n";
}

// print current timestamp
void print_timestamp()
{
    auto now = std::chrono::system_clock::to_time_t(
        std::chrono::system_clock::now());
    std::cout << "[Time] " << std::ctime(&now);
}

// ======================================================
// MAIN
// ======================================================

int main(void)
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    const int iterations = 1'000'000;
    std::string test_tag = "Tag";
    std::string test_msg = "Message";

    int call_count = 0;

    print_timestamp();
    warmup_logger();

    // Benchmark log_msg (std::string_view)
    print_header("String View Logger");
    {
        scoped_timer timer_sv("String View (Total)");
        for (int i = 0; i < iterations; ++i) {
            log_if_enabled(test_tag, test_msg);
            ++call_count;
        }
    }

    print_separator();

    // reset counter for clarity
    call_count = 0;

    // Benchmark log_msg_ (std::string copy)
    print_header("String Copy Logger");
    {
        scoped_timer timer_str("Normal String (Total)");
        for (int i = 0; i < iterations; ++i) {
            log_msg_(test_tag, test_msg);
            ++call_count;
        }
    }

    print_separator();

    // Inline logger benchmark
    print_header("Inline Logger");
    {
        scoped_timer timer_inline("Inline Logger (Total)");
        for (int i = 0; i < iterations; ++i) {
            log_msg_inline(test_tag, test_msg);
            ++call_count;
        }
    }

    flush_output();

    std::cout << "\nTotal log calls executed: "
              << call_count << '\n';

    print_counter("Final Call Count", call_count);

    // tiny derived metric
    std::cout << "Average logs per test: "
              << (call_count / 3) << '\n';

    // ===== EXTRA SMALL ADDITIONS (NEW) =====

    // Measure small sample block
    {
        scoped_timer t("Mini sample block");
        log_msg("Test", "Sample log");
    }

    // Simple stats example
    std::vector<int> samples = {10, 20, 30};
    int total = std::accumulate(samples.begin(), samples.end(), 0);
    std::cout << "Sample average: "
              << (total / samples.size()) << '\n';

    // Check if logging is enabled
    std::cout << "Logging enabled? "
              << (ENABLE_LOGGING ? "Yes" : "No") << '\n';

    // ===== FINAL TINY ADDITIONS =====

    // Find max and min sample
    auto [min_it, max_it] = std::minmax_element(samples.begin(), samples.end());
    std::cout << "Min sample: " << *min_it
              << ", Max sample: " << *max_it << '\n';

    // Count occurrences of a value
    int count_20 = std::count(samples.begin(), samples.end(), 20);
    std::cout << "Count of 20: " << count_20 << '\n';

    // Simple throughput estimate
    auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < 1000; ++i)
        log_msg_inline("T", "X");
    auto end = std::chrono::steady_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Throughput test (1000 logs): "
              << duration << " us\n";

    // ======================================

    return 0;
}
