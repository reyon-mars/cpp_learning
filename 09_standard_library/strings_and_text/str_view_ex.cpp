#include <string>
#include <string_view>
#include <iostream>
#include <chrono>
#include <ctime>
#include <numeric>
#include <vector>
#include <algorithm>
#include <ranges>
#include <format>
#include <concepts>
#include <functional>
#include <span>
#include <cassert>
#include "scoped_timer.hpp"

constexpr std::string_view DEFAULT_TAG   = "Default";
constexpr bool             ENABLE_LOGGING = true;

void log_msg(std::string_view tag, std::string_view msg) {
    std::cout << std::format("[{}]{}\n", tag, msg);
}

void log_msg_copy(std::string tag, std::string msg) {
    std::cout << std::format("[{}]{}\n", tag, msg);
}

void log_msg_inline(std::string_view tag, std::string_view msg) {
    std::cout << std::format("[{}]{}\n", tag, msg);
}

void log_if_enabled(std::string_view tag, std::string_view msg) {
    if constexpr (ENABLE_LOGGING)
        log_msg(tag, msg);
}

void spaced_log(std::string_view tag, std::string_view msg) {
    std::cout << '\n';
    log_msg(tag, msg);
}

template <std::invocable<std::string_view, std::string_view> Logger>
long long benchmark_logger(Logger&& fn, std::string_view tag,
                           std::string_view msg, int iterations) {
    return measure_us([&] {
        for ([[maybe_unused]] int i : std::views::iota(0, iterations))
            std::invoke(fn, tag, msg);
    });
}

[[nodiscard]] constexpr bool is_empty_message(std::string_view msg) noexcept {
    return msg.empty();
}

[[nodiscard]] std::size_t total_characters(std::span<const std::string> msgs) noexcept {
    return std::transform_reduce(msgs.begin(), msgs.end(),
                                 std::size_t{0}, std::plus<>{},
                                 [](const std::string& s) { return s.size(); });
}

void print_header(std::string_view title) {
    std::cout << std::format("\n--- {} ---\n", title);
}

void print_separator() {
    std::cout << "---------------------------------\n";
}

void print_timestamp() {
    const auto now  = std::chrono::system_clock::now();
    const auto time = std::chrono::system_clock::to_time_t(now);
    std::cout << std::format("[Time] {}", std::ctime(&time));
}

void print_note(std::string_view text) {
    std::cout << std::format("[Note] {}\n", text);
}

struct BenchmarkResult {
    std::string_view name;
    long long        elapsed_us;
};

void print_results(std::span<const BenchmarkResult> results) {
    std::cout << "\n--- Benchmark Summary ---\n";
    for (const auto& [name, us] : results)
        std::cout << std::format("  {:32s} {:>10} us\n", name, us);

    const auto fastest = std::ranges::min(results, {}, &BenchmarkResult::elapsed_us);
    std::cout << std::format("Fastest: {} ({} us)\n", fastest.name, fastest.elapsed_us);
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    constexpr int        iterations = 1'000'000;
    const std::string    test_tag   = "Tag";
    const std::string    test_msg   = "Message";

    print_timestamp();
    log_msg(DEFAULT_TAG, "Warmup");

    std::vector<BenchmarkResult> results;
    results.reserve(3);

    print_header("String View Logger");
    {
        ScopedTimer t{"String View (Total)"};
        results.push_back({"log_msg (string_view)",
                           benchmark_logger(log_msg, test_tag, test_msg, iterations)});
    }
    print_separator();

    print_header("String Copy Logger");
    {
        ScopedTimer t{"Normal String (Total)"};
        results.push_back({"log_msg_copy (string)",
                           benchmark_logger(log_msg_copy, test_tag, test_msg, iterations)});
    }
    print_separator();

    print_header("Inline Logger");
    {
        ScopedTimer t{"Inline Logger (Total)"};
        results.push_back({"log_msg_inline",
                           benchmark_logger(log_msg_inline, test_tag, test_msg, iterations)});
    }

    std::cout.flush();

    const int total_calls = static_cast<int>(results.size()) * iterations;
    std::cout << std::format("\nTotal log calls executed: {}\n", total_calls);
    std::cout << std::format("Average logs per test:    {}\n", total_calls / static_cast<int>(results.size()));

    print_results(results);

    std::cout << "\n--- Mini sample block ---\n";
    {
        ScopedTimer t{"Mini sample block"};
        log_msg("Test", "Sample log");
    }

    const std::vector<int> samples = {10, 20, 30};
    const auto [mn, mx] = std::ranges::minmax(samples);
    const int  avg      = std::reduce(samples.begin(), samples.end(), 0)
                          / static_cast<int>(samples.size());

    std::cout << std::format("Sample average: {}\n", avg);
    std::cout << std::format("Min sample: {}, Max sample: {}\n", mn, mx);
    std::cout << std::format("Count of 20: {}\n", std::ranges::count(samples, 20));
    std::cout << std::format("Logging enabled: {}\n", ENABLE_LOGGING ? "Yes" : "No");

    const long long throughput_us = measure_us([] {
        for ([[maybe_unused]] int i : std::views::iota(0, 1000))
            log_msg_inline("T", "X");
    });
    std::cout << std::format("Throughput test (1000 logs): {} us\n", throughput_us);

    std::cout << std::format("Empty message check: {}\n",
                             is_empty_message("") ? "Yes" : "No");

    spaced_log("Extra", "Spaced logger example");

    const std::vector<std::string> msg_list = {"Hello", "Logging", "Benchmark"};
    std::cout << std::format("Total characters in messages: {}\n",
                             total_characters(msg_list));

    print_note("Benchmark completed successfully");

    for (int i : std::views::iota(1, 4))
        log_msg_inline("Loop", std::format("Iteration {}", i));

    assert(!is_empty_message("hello"));
    assert(is_empty_message(""));
    assert(total_characters(msg_list) == 5 + 7 + 9);
    assert(std::ranges::is_sorted(results, {}, &BenchmarkResult::elapsed_us) ||
           !results.empty());

    std::cout << "\nAll assertions passed.\n";
    return 0;
}
