#include <iostream>
#include <chrono>
#include <string>
#include <string_view>
#include <utility>
#include <thread>
#include <vector>
#include <numeric>

class scoped_timer {
public:
    using Clock     = std::chrono::steady_clock;
    using TimePoint = Clock::time_point;

    explicit scoped_timer(std::string name)
        : start_{Clock::now()}, name_{std::move(name)} {}

    scoped_timer(const scoped_timer&)            = delete;
    scoped_timer& operator=(const scoped_timer&) = delete;

    scoped_timer(scoped_timer&& other) noexcept
        : start_{other.start_},
          name_{std::move(other.name_)},
          stopped_{std::exchange(other.stopped_, true)} {}

    scoped_timer& operator=(scoped_timer&& other) noexcept {
        if (this != &other) {
            stop();
            start_   = other.start_;
            name_    = std::move(other.name_);
            stopped_ = std::exchange(other.stopped_, true);
        }
        return *this;
    }

    ~scoped_timer() { stop(); }

    void stop() {
        if (stopped_) return;
        stopped_        = true;
        const auto us   = to_us(Clock::now() - start_);
        std::cout << name_ << ": " << us << " us (" << us / 1000 << " ms)\n";
    }

    void reset() noexcept {
        start_   = Clock::now();
        stopped_ = false;
    }

    [[nodiscard]] long long elapsed_us() const noexcept { return to_us(Clock::now() - start_); }
    [[nodiscard]] long long elapsed_ms() const noexcept { return elapsed_us() / 1000;           }
    [[nodiscard]] bool      is_stopped() const noexcept { return stopped_;                       }

    void print_elapsed() const {
        std::cout << "[Elapsed] " << elapsed_us() << " us\n";
    }

private:
    TimePoint   start_;
    std::string name_;
    bool        stopped_ = false;

    static long long to_us(Clock::duration d) noexcept {
        return std::chrono::duration_cast<std::chrono::microseconds>(d).count();
    }
};

template<typename Func>
void measure(std::string_view label, Func&& f) {
    using Clock = std::chrono::steady_clock;
    const auto begin = Clock::now();
    std::forward<Func>(f)();
    const auto us = std::chrono::duration_cast<std::chrono::microseconds>(
                        Clock::now() - begin).count();
    std::cout << label << ": " << us << " us\n";
}

template<typename Func>
[[nodiscard]] long long measure_return(Func&& f) {
    using Clock = std::chrono::steady_clock;
    const auto begin = Clock::now();
    std::forward<Func>(f)();
    return std::chrono::duration_cast<std::chrono::microseconds>(
               Clock::now() - begin).count();
}

void heavy_task() {
    const std::vector<int> values(100'000, 1);
    [[maybe_unused]] volatile int sum =
        std::accumulate(values.cbegin(), values.cend(), 0);
}

static void busy(long long n) {
    for (volatile long long i = 0; i < n; ++i);
}

int main() {
    std::cout << "--- RAII blocks ---\n";
    { scoped_timer timer{"Block 1"}; busy(1'000'000); }
    { scoped_timer timer{"Block 2"}; busy(2'000'000); }

    std::cout << "\n--- Manual stop + elapsed ---\n";
    {
        scoped_timer timer{"Manual stop"};
        busy(500'000);
        std::cout << "Mid elapsed=" << timer.elapsed_us() << " us\n";
        timer.print_elapsed();
        timer.stop();
    }

    std::cout << "\n--- measure (free function) ---\n";
    measure("Quick measure", [] { busy(300'000); });

    std::cout << "\n--- is_stopped ---\n";
    {
        scoped_timer timer{"Check state"};
        busy(100'000);
        std::cout << "is_stopped=" << std::boolalpha << timer.is_stopped() << "\n";
    }

    std::cout << "\n--- measure_return ---\n";
    const long long t = measure_return([] { busy(150'000); });
    std::cout << "Returned=" << t << " us\n";

    std::cout << "\n--- reset ---\n";
    {
        scoped_timer timer{"Reset demo"};
        std::this_thread::sleep_for(std::chrono::milliseconds{20});
        std::cout << "Before reset=" << timer.elapsed_ms() << " ms\n";
        timer.reset();
        std::this_thread::sleep_for(std::chrono::milliseconds{10});
        std::cout << "After reset=" << timer.elapsed_ms() << " ms\n";
    }

    std::cout << "\n--- Heavy task ---\n";
    measure("Vector accumulate", heavy_task);

    std::cout << "\n--- Move assignment ---\n";
    scoped_timer t1{"Timer A"};
    std::this_thread::sleep_for(std::chrono::milliseconds{5});
    scoped_timer t2{"Timer B"};
    t2 = std::move(t1);
    std::this_thread::sleep_for(std::chrono::milliseconds{5});

    return 0;
}
