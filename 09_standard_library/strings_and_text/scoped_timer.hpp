#include <iostream>
#include <chrono>
#include <string>
#include <thread>
#include <numeric>
#include <vector>
#include <algorithm>
#include <format>
#include <ranges>
#include <span>
#include <cassert>
#include <functional>
#include <concepts>

using Clock     = std::chrono::steady_clock;
using TimePoint = Clock::time_point;
using us        = std::chrono::microseconds;
using ms        = std::chrono::milliseconds;
using sec       = std::chrono::seconds;

class ScopedTimer {
    TimePoint   start_;
    std::string name_;

public:
    explicit ScopedTimer(std::string name)
        : start_(Clock::now()), name_(std::move(name)) {}

    ~ScopedTimer() {
        const auto elapsed = Clock::now() - start_;
        std::cout << std::format("{}: {} us ({} ms, {} s)\n",
                                 name_,
                                 duration_cast<us>(elapsed).count(),
                                 duration_cast<ms>(elapsed).count(),
                                 duration_cast<sec>(elapsed).count());
    }

    ScopedTimer(const ScopedTimer&)            = delete;
    ScopedTimer& operator=(const ScopedTimer&) = delete;
};

[[nodiscard]] auto elapsed_us(TimePoint start) noexcept {
    return std::chrono::duration_cast<us>(Clock::now() - start).count();
}

void busy_work(int n) noexcept {
    volatile int x = 0;
    for (int i = 0; i < n; ++i)
        x += i;
}

template <std::invocable Fn>
long long measure_us(Fn&& fn) {
    const auto start = Clock::now();
    std::invoke(std::forward<Fn>(fn));
    return elapsed_us(start);
}

template <std::invocable Fn>
void timed(std::string_view label, Fn&& fn) {
    ScopedTimer t{std::string(label)};
    std::invoke(std::forward<Fn>(fn));
}

void timed_sleep(std::string_view label, int ms_count) {
    timed(label, [ms_count] {
        std::this_thread::sleep_for(ms(ms_count));
    });
}

void print_current_time() {
    const auto now  = std::chrono::system_clock::now();
    const auto time = std::chrono::system_clock::to_time_t(now);
    std::cout << std::format("[Current time] {}", std::ctime(&time));
}

struct TimingStats {
    long long min{}, max{}, total{};
    double    mean{};
    std::size_t count{};
};

[[nodiscard]] TimingStats compute_stats(std::span<const long long> times) noexcept {
    if (times.empty()) return {};
    auto [mn, mx] = std::ranges::minmax(times);
    const long long sum = std::reduce(times.begin(), times.end(), 0LL);
    return {mn, mx, sum, static_cast<double>(sum) / static_cast<double>(times.size()), times.size()};
}

void print_stats(const TimingStats& s) {
    std::cout << std::format("  count={}, min={} us, max={} us, mean={:.1f} us, total={} us\n",
                             s.count, s.min, s.max, s.mean, s.total);
}

void print_times(std::span<const long long> times, std::string_view label = "times") {
    std::cout << std::format("{}: ", label);
    for (long long t : times) std::cout << std::format("{} ", t);
    std::cout << '\n';
}

int main() {
    std::cout << "--- Scoped Timer Demo ---\n";
    print_current_time();

    ScopedTimer total{"total program time"};

    timed("main workload",   [] { busy_work(300'000); });
    timed("inner workload",  [] { busy_work(150'000); });
    timed("helper workload", [] { busy_work(200'000); });

    int work_count = 0;
    for (std::string_view label : {"counted workload 1", "counted workload 2"}) {
        timed(label, [&] { busy_work(100'000); ++work_count; });
    }

    timed("repeat workload x3", [] {
        for ([[maybe_unused]] int i : std::views::iota(0, 3))
            busy_work(80'000);
    });

    timed_sleep("sleep test (200ms)", 200);

    std::cout << std::format("Work functions executed: {}\n", work_count);
    std::cout << std::format("Clock is steady: {}\n",
                             Clock::is_steady ? "Yes" : "No");

    std::cout << "\n--- Loop workloads ---\n";
    for (int i : std::views::iota(1, 4)) {
        timed(std::format("loop workload {}", i),
              [i] { busy_work(30'000 * i); });
    }

    std::cout << "\n--- Comparison workloads ---\n";
    timed("workload A", [] { busy_work( 60'000); });
    timed("workload B", [] { busy_work(120'000); });

    std::cout << "\n--- Throughput ---\n";
    constexpr int ops = 100'000;
    const long long dur = measure_us([] { busy_work(ops); });
    std::cout << std::format("Duration: {} us | Throughput: {} ops/us\n",
                             dur, ops / std::max(dur, 1LL));

    std::cout << "\n--- Timing Statistics ---\n";
    std::vector<long long> sample_times;
    sample_times.reserve(8);
    for (int n : {50'000, 100'000, 75'000, 90'000, 60'000, 110'000, 80'000, 95'000})
        sample_times.push_back(measure_us([n] { busy_work(n); }));

    print_times(sample_times, "raw sample times (us)");
    print_stats(compute_stats(sample_times));

    std::ranges::sort(sample_times);
    print_times(sample_times, "sorted sample times (us)");

    const long long median = sample_times[sample_times.size() / 2];
    std::cout << std::format("Median: {} us\n", median);

    std::cout << "\n--- quick_inline block ---\n";
    timed("quick inline block", [] { busy_work(50'000); });

    timed_sleep("tiny sleep (50ms)", 50);

    std::cout << std::format("Number of samples: {}\n", sample_times.size());

    assert(Clock::is_steady);
    assert(!sample_times.empty());
    assert(std::ranges::is_sorted(sample_times));

    std::cout << "\nAll assertions passed.\n--- Program Finished ---\n";
    return 0;
}

//nothing
