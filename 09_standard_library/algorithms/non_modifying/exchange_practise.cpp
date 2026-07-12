#include <iostream>
#include <utility>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cstdlib>
#include <stdexcept>
#include <span>
#include <format>
#include <ranges>
#include <cassert>
#include <array>
#include <functional>

class Counter {
    int value_{0};
public:
    explicit Counter(int initial = 0) noexcept : value_{initial} {}

    [[nodiscard]] int  current()           const noexcept { return value_; }
    [[nodiscard]] bool is_zero()           const noexcept { return value_ == 0; }
    [[nodiscard]] bool is_positive()       const noexcept { return value_ > 0; }
    [[nodiscard]] bool equals(int other)   const noexcept { return value_ == other; }

    int increment(int amount) noexcept {
        return std::exchange(value_, value_ + amount);
    }
    int decrement(int amount) noexcept {
        return std::exchange(value_, value_ - amount);
    }
    int reset() noexcept {
        return std::exchange(value_, 0);
    }

    void set(int v)                      noexcept { value_  = v; }
    void add(int amount)                 noexcept { value_ += amount; }
    void add_multiple(int times, int amount) noexcept { value_ += times * amount; }
    void multiply(int factor)            noexcept { value_ *= factor; }
    void make_positive()                 noexcept { value_  = std::abs(value_); }
    void increment_loop(int times)       noexcept { value_ += times; }
    void max_with(int other)             noexcept { value_  = std::max(value_, other); }
    void min_with(int other)             noexcept { value_  = std::min(value_, other); }

    [[nodiscard]] bool divide(int divisor) noexcept {
        if (divisor == 0) return false;
        value_ /= divisor;
        return true;
    }

    void clamp(int lo, int hi) {
        if (lo > hi) throw std::invalid_argument("clamp: lo must be <= hi");
        value_ = std::clamp(value_, lo, hi);
    }

    void apply(std::span<const int> deltas) noexcept {
        value_ = std::reduce(deltas.begin(), deltas.end(), value_);
    }

    template <std::invocable<int> Fn>
    void transform(Fn&& fn) noexcept(noexcept(fn(value_))) {
        value_ = std::invoke(std::forward<Fn>(fn), value_);
    }

    void print() const {
        std::cout << std::format("Counter value: {}\n", value_);
    }

    friend std::ostream& operator<<(std::ostream& os, const Counter& c) {
        return os << c.value_;
    }

    auto operator<=>(const Counter&) const = default;
};

class TrackedCounter : public Counter {
    std::vector<int> history_;
public:
    explicit TrackedCounter(int initial = 0) : Counter(initial) {
        history_.push_back(current());
    }

    void record() { history_.push_back(current()); }

    [[nodiscard]] std::span<const int> history() const noexcept { return history_; }

    [[nodiscard]] int peak() const {
        return *std::ranges::max_element(history_);
    }

    [[nodiscard]] double average() const {
        if (history_.empty()) return 0.0;
        return static_cast<double>(std::reduce(history_.begin(), history_.end(), 0))
               / static_cast<double>(history_.size());
    }

    void print_history() const {
        std::cout << "History: ";
        for (int v : history_) std::cout << std::format("{} ", v);
        std::cout << '\n';
    }
};

int main() {
    Counter c;

    for (int i : std::views::iota(1, 100))
        std::cout << std::format("{} ", c.increment(i));
    std::cout << std::format("\nAfter reset: {}\n", c.reset());

    std::cout << std::format("Current value: {}\n", c.current());
    std::cout << std::format("Is counter zero? {}\n", c.is_zero() ? "Yes" : "No");

    c.increment(10); c.print();
    c.reset();       c.print();

    c.increment(20);
    c.decrement(5);  c.print();

    c.add_multiple(3, 2); c.print();
    std::cout << std::format("Is positive? {}\n", c.is_positive() ? "Yes" : "No");

    c.set(10);   c.print();
    c.multiply(3); c.print();
    c.add(7);    c.print();

    std::cout << "\n--- Extra Counter Operations ---\n";

    if (c.divide(2))  std::cout << "Division successful\n";
    c.print();
    if (!c.divide(0)) std::cout << "Division by zero prevented\n";

    c.set(-50);      c.print();
    c.make_positive(); c.print();
    std::cout << std::format("Equals 50? {}\n", c.equals(50) ? "Yes" : "No");

    c.increment_loop(5); c.print();
    c.max_with(100);     c.print();

    constexpr std::array nums{1, 2, 3, 4, 5};
    std::cout << std::format("Accumulated total: {}\n",
                             std::reduce(nums.begin(), nums.end(), 0));
    std::cout << std::format("Largest element: {}\n",
                             *std::ranges::max_element(nums));

    std::cout << "\n--- New Operations Demo ---\n";
    c.set(200);
    c.min_with(150); c.print();

    c.set(30);
    c.clamp(50, 100); c.print();

    const std::array deltas{5, -3, 10, 2};
    c.apply(deltas); c.print();

    std::cout << std::format("Counter via stream: {}\n", c.current());

    Counter a{10}, b{20};
    std::cout << std::format("a < b? {}\n", (a < b) ? "Yes" : "No");
    std::cout << std::format("a == a? {}\n", (a == a) ? "Yes" : "No");

    std::cout << "\n--- transform ---\n";
    c.set(7);
    c.transform([](int x) noexcept { return x * x; });
    std::cout << std::format("7 squared via transform: {}\n", c.current());

    c.transform([](int x) noexcept { return x + 1; });
    std::cout << std::format("+1 via transform: {}\n", c.current());

    std::cout << "\n--- TrackedCounter ---\n";
    {
        TrackedCounter tc(0);
        for (int delta : {10, -3, 7, 20, -5}) {
            tc.add(delta);
            tc.record();
        }
        tc.print_history();
        std::cout << std::format("Peak: {}\n",    tc.peak());
        std::cout << std::format("Average: {:.2f}\n", tc.average());
    }

    std::cout << "\n--- views pipeline on counter sequence ---\n";
    {
        std::vector<int> steps;
        steps.reserve(10);
        Counter seq(0);
        for (int i : std::views::iota(1, 11)) {
            seq.increment(i);
            steps.push_back(seq.current());
        }

        auto pipeline = steps
            | std::views::filter([](int x) { return x % 2 == 0; })
            | std::views::transform([](int x) { return x * 2; });

        std::cout << "Even values doubled: ";
        for (int v : pipeline) std::cout << std::format("{} ", v);
        std::cout << '\n';
    }

    assert(Counter(5).current() == 5);
    assert(Counter(0).is_zero());
    assert(!Counter(-1).is_positive());
    assert([]{ Counter c; c.increment(3); return c.equals(3); }());
    assert([]{ Counter c(6); c.divide(2); return c.equals(3); }());
    assert([]{ Counter c(10); c.clamp(0, 8); return c.equals(8); }());
    assert(a < b);
    assert(!(b < a));

    std::cout << "\nAll assertions passed.\n";
    return 0;
}
