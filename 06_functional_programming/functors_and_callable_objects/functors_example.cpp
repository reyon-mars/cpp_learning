#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <functional>
#include <ranges>
#include <span>
#include <string>
#include <string_view>
#include <cassert>
#include <utility>
#include <concepts>

class Adder {
    int value_;
public:
    explicit Adder(int v) noexcept : value_(v) {}
    [[nodiscard]] int operator()(int x) const noexcept { return x + value_; }
};

class Counter {
    int count_ = 0;
public:
    void operator()() {
        std::cout << "Call count: " << ++count_ << '\n';
    }
    [[nodiscard]] int get_count() const noexcept { return count_; }
};

class GreaterThan {
    int threshold_;
public:
    explicit GreaterThan(int t) noexcept : threshold_(t) {}
    [[nodiscard]] bool operator()(int x) const noexcept { return x > threshold_; }
};

template<typename T>
class Multiplier {
    T factor_;
public:
    explicit Multiplier(T f) noexcept : factor_(std::move(f)) {}
    [[nodiscard]] T operator()(T x) const noexcept { return x * factor_; }
};

class IsEven {
public:
    [[nodiscard]] bool operator()(int x) const noexcept { return x % 2 == 0; }
};

class Square {
public:
    [[nodiscard]] int operator()(int x) const noexcept { return x * x; }
};

class InRange {
    int low_, high_;
public:
    InRange(int low, int high) noexcept : low_(low), high_(high) {}
    [[nodiscard]] bool operator()(int x) const noexcept {
        return x >= low_ && x <= high_;
    }
};

class Formatter {
    std::string prefix_;
public:
    explicit Formatter(std::string prefix) : prefix_(std::move(prefix)) {}
    void operator()(int x) const {
        std::cout << prefix_ << x << '\n';
    }
};

template<std::invocable<int> Func>
[[nodiscard]] auto execute(Func&& f, int value) {
    return std::invoke(std::forward<Func>(f), value);
}

int main() {
    std::vector<int> values = {1, 2, 3, 4, 5};

    std::ranges::transform(values, values.begin(), Adder(10));
    for (int v : values) std::cout << v << ' ';
    std::cout << '\n';

    Counter counter;
    counter();
    counter();
    counter();

    std::cout << "\n--- Filtering with predicate ---\n";
    auto it = std::ranges::find_if(values, GreaterThan(12));
    if (it != values.end())
        std::cout << "First value > 12: " << *it << '\n';

    std::cout << "\n--- Sorting with functor ---\n";
    std::ranges::sort(values, std::greater<>{});
    for (int v : values) std::cout << v << ' ';
    std::cout << '\n';

    std::cout << "\n--- Templated functor ---\n";
    Multiplier<int> mult(3);
    std::cout << "5 * 3 = " << mult(5) << '\n';

    std::cout << "\n--- for_each with Counter ---\n";
    Counter algoCounter;
    std::ranges::for_each(values, [&](int) { algoCounter(); });

    const int total = std::accumulate(values.begin(), values.end(), 0, std::plus<>{});
    std::cout << "\nTotal sum: " << total << '\n';

    const auto count_gt_12 = std::ranges::count_if(values, GreaterThan(12));
    std::cout << "Count > 12: " << count_gt_12 << '\n';

    std::cout << "\n--- Applying multiplier to all ---\n";
    std::ranges::for_each(values, [&](int v) { std::cout << mult(v) << ' '; });
    std::cout << '\n';

    std::cout << "\n--- Extra Functors ---\n";

    const int diff = std::accumulate(values.begin(), values.end(), 0, std::minus<>{});
    std::cout << "Accumulated subtraction: " << diff << '\n';

    const auto even_count = std::ranges::count_if(values, IsEven{});
    std::cout << "Even count: " << even_count << '\n';

    std::cout << "Printer functor: ";
    std::ranges::for_each(values, [](int x) { std::cout << x << ' '; });
    std::cout << '\n';

    std::cout << "\n--- Advanced Callable Objects ---\n";

    std::vector<int> squared(values.size());
    std::ranges::transform(values, squared.begin(), Square{});
    std::cout << "Squared: ";
    for (int x : squared) std::cout << x << ' ';
    std::cout << '\n';

    const auto in_range = std::ranges::count_if(values, InRange(12, 14));
    std::cout << "Values in [12,14]: " << in_range << '\n';

    std::cout << "\nFormatted output:\n";
    std::ranges::for_each(values, Formatter("Value = "));

    std::cout << "\nExecute Adder(20) on 5:       " << execute(Adder(20),           5) << '\n';
    std::cout << "Execute Multiplier(4) on 3:   " << execute(Multiplier<int>(4),   3) << '\n';
    std::cout << "Execute Square on 7:           " << execute(Square{},             7) << '\n';

    std::function<int(int)> callable = Multiplier<int>(5);
    std::cout << "std::function result:          " << callable(6) << '\n';

    std::cout << "Counter final state: " << counter.get_count() << '\n';

    assert(mult(2) == 6);
    assert(even_count >= 0);
    assert(execute(Adder(10), 5) == 15);
    assert(InRange(1, 10)(5));
    assert(!IsEven{}(3));

    std::cout << "\nAll assertions passed.\n";
    return 0;
}
