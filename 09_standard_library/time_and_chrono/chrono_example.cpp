#include <iostream>
#include <chrono>
#include <thread>
#include <numeric>
#include <vector>
#include <algorithm>
#include <ranges>
#include <format>

using namespace std::chrono;
using namespace std::chrono_literals;

template <typename Clock>
struct Stopwatch {
    time_point<Clock> origin{Clock::now()};

    void reset() noexcept { origin = Clock::now(); }

    template <typename Unit = milliseconds>
    [[nodiscard]] Unit elapsed() const noexcept {
        return duration_cast<Unit>(Clock::now() - origin);
    }
};

template <std::invocable F>
auto bench(F&& fn) -> microseconds {
    Stopwatch<high_resolution_clock> sw;
    std::forward<F>(fn)();
    return sw.elapsed<microseconds>();
}

int main() {
    constexpr auto five_seconds = 5s;
    std::cout << std::format("Duration: {} seconds\n", five_seconds.count());

    Stopwatch<high_resolution_clock> sw;
    std::this_thread::sleep_for(100ms);
    const auto elapsed_ms = sw.elapsed<milliseconds>();
    std::cout << std::format("Elapsed: {} ms\n", elapsed_ms.count());

    Stopwatch<steady_clock> steady_sw;
    std::this_thread::sleep_for(50ms);
    const auto steady_us = steady_sw.elapsed<microseconds>();
    std::cout << std::format("Steady clock elapsed: {} us\n", steady_us.count());

    const auto now        = system_clock::now();
    const auto now_time_t = system_clock::to_time_t(now);
    std::cout << std::format("Current system time: {}", std::ctime(&now_time_t));

    std::cout << std::format("Elapsed in seconds: {} s\n",
                             duration_cast<seconds>(elapsed_ms).count());

    std::cout << std::format("Elapsed in nanoseconds: {} ns\n",
                             duration_cast<nanoseconds>(elapsed_ms).count());

    std::cout << std::format("Elapsed > 50ms? {}\n", elapsed_ms > 50ms ? "Yes" : "No");

    const auto total_time = elapsed_ms + 50ms;
    std::cout << std::format("Elapsed + 50ms: {} ms\n", total_time.count());

    const auto loop_time = bench([] {
        volatile int sink{};
        for (int i = 0; i < 1'000'000; ++i) sink = i;
    });
    std::cout << std::format("Loop execution time: {} us\n", loop_time.count());

    std::cout << std::format("steady_clock is steady: {}\n",
                             steady_clock::is_steady ? "Yes" : "No");

    const auto seconds_epoch = duration_cast<seconds>(now.time_since_epoch());
    std::cout << std::format("Seconds since epoch: {}\n", seconds_epoch.count());

    std::this_thread::sleep_for(10ms);
    std::cout << "Slept for additional 10ms\n";

    const auto avg_time = (elapsed_ms + 50ms) / 2;
    std::cout << std::format("Average duration: {} ms\n", avg_time.count());

    const std::vector<milliseconds> durations {
        elapsed_ms,
        duration_cast<milliseconds>(steady_us),
        total_time,
        duration_cast<milliseconds>(loop_time),
    };

    const auto sample_sum = std::accumulate(
        durations.begin(), durations.end(), 0ms);
    std::cout << std::format("Sample duration sum: {} ms\n", sample_sum.count());

    const auto [min_it, max_it] = std::ranges::minmax_element(durations);
    std::cout << std::format("Min sample: {} ms\n", min_it->count());
    std::cout << std::format("Max sample: {} ms\n", max_it->count());

    const auto count_large = std::ranges::count_if(
        durations, [](const milliseconds& d) { return d > 50ms; });
    std::cout << std::format("Samples > 50ms: {}\n", count_large);

    std::cout << "All samples (ms): ";
    for (const auto& d : durations) std::cout << d.count() << ' ';
    std::cout << '\n';

    return 0;
}
