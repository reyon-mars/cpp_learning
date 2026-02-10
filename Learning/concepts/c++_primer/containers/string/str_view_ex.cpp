#include <string>
#include <string_view>
#include <iostream>
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

// default tag (compile-time)
constexpr std::string_view DEFAULT_TAG = "Default";

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

// ====================================

// ======================================================
// MAIN
// ======================================================

int main(void)
{
    // ---- tiny performance-related addition ----
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    // -------------------------------------------

    const int iterations = 1'000'000;
    std::string test_tag = "Tag";
    std::string test_msg = "Message";

    int call_count = 0;

    // ---- VERY TINY EXTRA ----
    warmup_logger();
    // -------------------------

    // Benchmark log_msg (std::string_view)
    print_header("String View Logger");
    {
        scoped_timer timer_sv("String View (Total)");
        for (int i = 0; i < iterations; ++i) {
            log_msg(test_tag, test_msg);
            ++call_count;
        }
    }

    // Benchmark log_msg_ (std::string copy)
    print_header("String Copy Logger");
    {
        scoped_timer timer_str("Normal String (Total)");
        for (int i = 0; i < iterations; ++i) {
            log_msg_(test_tag, test_msg);
            ++call_count;
        }
    }

    // ---- very small added benchmark ----
    print_header("Inline Logger");
    {
        scoped_timer timer_inline("Inline Logger (Total)");
        for (int i = 0; i < iterations; ++i) {
            log_msg_inline(test_tag, test_msg);
            ++call_count;
        }
    }
    // -----------------------------------

    flush_output();

    std::cout << "\nTotal log calls executed: "
              << call_count << '\n';

    // ---- tiny extra output ----
    print_counter("Final Call Count", call_count);
    // ---------------------------

    return 0;
}
 
