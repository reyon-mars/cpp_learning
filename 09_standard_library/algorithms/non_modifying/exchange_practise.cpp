#include <iostream>
#include <utility>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cstdlib>
#include <stdexcept>
#include <span>

class Counter {
    int value_{0};

public:
    explicit Counter(int initial = 0) noexcept : value_{initial} {}

    [[nodiscard]] int current() const noexcept { return value_; }
    [[nodiscard]] bool is_zero()     const noexcept { return value_ == 0; }
    [[nodiscard]] bool is_positive() const noexcept { return value_ >  0; }
    [[nodiscard]] bool equals(int other) const noexcept { return value_ == other; }

    int increment(int amount) noexcept {
        return std::exchange(value_, value_ + amount);
    }

    int decrement(int amount) noexcept {
        return std::exchange(value_, value_ - amount);
    }

    int reset() noexcept {
        return std::exchange(value_, 0);
    }

    void set(int v) noexcept { value_ = v; }

    void add(int amount) noexcept { value_ += amount; }

    void add_multiple(int times, int amount) noexcept {
        value_ += times * amount;
    }

    void multiply(int factor) noexcept { value_ *= factor; }

    [[nodiscard]] bool divide(int divisor) noexcept {
        if (divisor == 0) return false;
        value_ /= divisor;
        return true;
    }

    void make_positive() noexcept { value_ = std::abs(value_); }

    void increment_loop(int times) noexcept {
        value_ += times;
    }

    void max_with(int other) noexcept {
        value_ = std::max(value_, other);
    }

    void min_with(int other) noexcept {
        value_ = std::min(value_, other);
    }

    void clamp(int lo, int hi) {
        if (lo > hi) throw std::invalid_argument("clamp: lo must be <= hi");
        value_ = std::clamp(value_, lo, hi);
    }

    void apply(std::span<const int> deltas) noexcept {
        value_ = std::accumulate(deltas.begin(), deltas.end(), value_);
    }

    void print() const {
        std::cout << "Counter value: " << value_ << '\n';
    }

    friend std::ostream& operator<<(std::ostream& os, const Counter& c) {
        return os << c.value_;
    }

    auto operator<=>(const Counter&) const = default;
};

int main() {
    Counter c;

    for (int i = 1; i < 100; ++i) {
        std::cout << c.increment(i) << ' ';
    }
    std::cout << c.reset() << '\n';

    std::cout << "Current value: " << c.current() << '\n';
    std::cout << "Is counter zero? " << (c.is_zero() ? "Yes" : "No") << '\n';

    c.increment(10);
    c.print();

    c.reset();
    c.print();

    c.increment(20);
    c.decrement(5);
    c.print();

    c.add_multiple(3, 2);
    c.print();

    std::cout << "Is positive? " << (c.is_positive() ? "Yes" : "No") << '\n';

    c.set(10);
    c.print();

    c.multiply(3);
    c.print();

    c.add(7);
    c.print();

    std::cout << "\n--- Extra Counter Operations ---\n";

    if (c.divide(2)) {
        std::cout << "Division successful\n";
    }
    c.print();

    if (!c.divide(0)) {
        std::cout << "Division by zero prevented\n";
    }

    c.set(-50);
    c.print();

    c.make_positive();
    c.print();

    std::cout << "Equals 50? " << (c.equals(50) ? "Yes" : "No") << '\n';

    c.increment_loop(5);
    c.print();

    c.max_with(100);
    c.print();

    constexpr std::array nums{1, 2, 3, 4, 5};

    const int total = std::accumulate(nums.begin(), nums.end(), 0);
    std::cout << "Accumulated total: " << total << '\n';

    const auto max_it = std::max_element(nums.begin(), nums.end());
    std::cout << "Largest element: " << *max_it << '\n';

    std::cout << "\n--- New Operations Demo ---\n";

    c.set(200);
    c.min_with(150);
    c.print();

    c.set(30);
    c.clamp(50, 100);
    c.print();

    const std::array deltas{5, -3, 10, 2};
    c.apply(deltas);
    c.print();

    std::cout << "Counter via stream: " << c << '\n';

    Counter a{10}, b{20};
    std::cout << "a < b? " << (a < b ? "Yes" : "No") << '\n';

    return 0;
}
