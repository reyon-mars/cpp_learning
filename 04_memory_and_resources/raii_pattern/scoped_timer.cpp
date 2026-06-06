#include <iostream>
#include <chrono>
#include <string>
#include <string_view>
#include <thread>
#include <utility>
#include <vector>
#include <numeric>

class scoped_timer {
public:
    using Clock    = std::chrono::steady_clock;
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
        stopped_     = true;
        const auto end = Clock::now();
        const auto us  = to_us(end - start_);
        const auto ms  = us / 1000;
        std::cout << name_ << ": " << us << " us (" << ms << " ms)\n";
    }

    void restart() noexcept {
        start_   = Clock::now();
        stopped_ = false;
    }

    [[nodiscard]] long long elapsed_us() const noexcept {
        return to_us(Clock::now() - start_);
    }

    [[nodiscard]] long long elapsed_ms() const noexcept {
        return elapsed_us() / 1000;
    }

    [[nodiscard]] bool is_stopped() const noexcept { return stopped_; }

    void print_elapsed() const {
        std::cout << "[Elapsed] " << elapsed_us() << " us\n";
    }

    template<typename Func>
    static void measure(std::string_view label, Func&& f) {
        const auto begin = Clock::now();
        std::forward<Func>(f)();
        std::cout << label << ": " << to_us(Clock::now() - begin) << " us\n";
    }

    template<typename Func>
    [[nodiscard]] static long long measure_return(Func&& f) {
        const auto begin = Clock::now();
        std::forward<Func>(f)();
        return to_us(Clock::now() - begin);
    }

    static void sleep_ms(int ms) {
        std::this_thread::sleep_for(std::chrono::milliseconds{ms});
    }

private:
    TimePoint   start_;
    std::string name_;
    bool        stopped_ = false;

    static long long to_us(Clock::duration d) noexcept {
        return std::chrono::duration_cast<std::chrono::microseconds>(d).count();
    }
};

void heavy_task() {
    const std::vector<int> values(100'000, 1);
    [[maybe_unused]] volatile int sum =
        std::accumulate(values.cbegin(), values.cend(), 0);
}

int main() {
    std::cout << "--- RAII stop on scope exit ---\n";
    {
        scoped_timer timer{"Sleep 100ms"};
        scoped_timer::sleep_ms(100);
    }

    std::cout << "\n--- Manual stop ---\n";
    {
        scoped_timer timer{"Manual stop"};
        scoped_timer::sleep_ms(50);
        timer.stop();
    }

    std::cout << "\n--- elapsed_us mid-flight ---\n";
    {
        scoped_timer timer{"Elapsed check"};
        scoped_timer::sleep_ms(30);
        std::cout << "Mid elapsed=" << timer.elapsed_us() << " us\n";
    }

    std::cout << "\n--- restart ---\n";
    {
        scoped_timer timer{"Restart demo"};
        scoped_timer::sleep_ms(20);
        timer.restart();
        scoped_timer::sleep_ms(20);
    }

    std::cout << "\n--- measure (static) ---\n";
    scoped_timer::measure("Lambda task", [] { scoped_timer::sleep_ms(40); });

    std::cout << "\n--- is_stopped ---\n";
    {
        scoped_timer timer{"Stopped flag"};
        scoped_timer::sleep_ms(10);
        std::cout << "is_stopped=" << std::boolalpha << timer.is_stopped() << "\n";
    }

    std::cout << "\n--- measure_return ---\n";
    const auto duration = scoped_timer::measure_return(
        [] { scoped_timer::sleep_ms(25); });
    std::cout << "Returned=" << duration << " us\n";

    std::cout << "\n--- print_elapsed ---\n";
    {
        scoped_timer timer{"Print elapsed"};
        scoped_timer::sleep_ms(15);
        timer.print_elapsed();
    }

    std::cout << "\n--- Heavy task ---\n";
    scoped_timer::measure("Vector accumulate", heavy_task);

    std::cout << "\n--- Move assignment ---\n";
    scoped_timer t1{"Timer A"};
    scoped_timer::sleep_ms(5);
    scoped_timer t2{"Timer B"};
    t2 = std::move(t1);
    scoped_timer::sleep_ms(5);

    return 0;
}
