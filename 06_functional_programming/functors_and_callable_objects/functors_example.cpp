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
#include <format>
#include <optional>
#include <type_traits>

class Adder {
    int value_;
public:
    explicit Adder(int v) noexcept : value_(v) {}
    [[nodiscard]] int operator()(int x) const noexcept { return x + value_; }
    [[nodiscard]] Adder operator+(const Adder& other) const noexcept {
        return Adder(value_ + other.value_);
    }
};

class Counter {
    int count_ = 0;
public:
    void operator()() {
        std::cout << std::format("Call count: {}\n", ++count_);
    }
    [[nodiscard]] int get_count() const noexcept { return count_; }
    void reset() noexcept { count_ = 0; }
};

class GreaterThan {
    int threshold_;
public:
    explicit GreaterThan(int t) noexcept : threshold_(t) {}
    [[nodiscard]] bool operator()(int x) const noexcept { return x > threshold_; }
};

template <typename T>
class Multiplier {
    T factor_;
public:
    explicit Multiplier(T f) noexcept : factor_(std::move(f)) {}
    [[nodiscard]] T operator()(T x) const noexcept { return x * factor_; }
    [[nodiscard]] T factor() const noexcept { return factor_; }
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
        std::cout << std::format("{}{}\n", prefix_, x);
    }
};

class Clamp {
    int lo_, hi_;
public:
    Clamp(int lo, int hi) noexcept : lo_(lo), hi_(hi) {}
    [[nodiscard]] int operator()(int x) const noexcept {
        return std::clamp(x, lo_, hi_);
    }
};

template <std::predicate<int> P>
class Negated {
    P pred_;
public:
    explicit Negated(P p) : pred_(std::move(p)) {}
    [[nodiscard]] bool operator()(int x) const { return !pred_(x); }
};

template <std::predicate<int> A, std::predicate<int> B>
class Both {
    A a_;
    B b_;
public:
    Both(A a, B b) : a_(std::move(a)), b_(std::move(b)) {}
    [[nodiscard]] bool operator()(int x) const { return a_(x) && b_(x); }
};

template <std::invocable<int> Func>
[[nodiscard]] auto execute(Func&& f, int value) {
    return std::invoke(std::forward<Func>(f), value);
}

template <std::invocable<int> F, std::invocable<int> G>
[[nodiscard]] auto compose(F&& f, G&& g) {
    return [f = std::forward<F>(f), g = std::forward<G>(g)](int x) {
        return std::invoke(f, std::invoke(g, x));
    };
}

template <std::invocable<int> Fn>
[[nodiscard]] std::vector<int> transform_copy(std::span<const int> src, Fn&& fn) {
    std::vector<int> out(src.size());
    std::ranges::transform(src, out.begin(), std::forward<Fn>(fn));
    return out;
}

template <std::predicate<int> Pred>
[[nodiscard]] std::optional<int> find_first(std::span<const int> src, Pred&& pred) {
    const auto it = std::ranges::find_if(src, std::forward<Pred>(pred));
    return (it != src.end()) ? std::optional{*it} : std::nullopt;
}

int main() {
    std::vector<int> values = {1, 2, 3, 4, 5};

    std::ranges::transform(values, values.begin(), Adder(10));
    for (int v : values) std::cout << std::format("{} ", v);
    std::cout << '\n';

    Counter counter;
    counter();
    counter();
    counter();

    std::cout << "\n--- Filtering with predicate ---\n";
    if (const auto v = find_first(values, GreaterThan(12)))
        std::cout << std::format("First value > 12: {}\n", *v);

    std::cout << "\n--- Sorting with functor ---\n";
    std::ranges::sort(values, std::greater<>{});
    for (int v : values) std::cout << std::format("{} ", v);
    std::cout << '\n';

    std::cout << "\n--- Templated functor ---\n";
    Multiplier<int> mult(3);
    std::cout << std::format("5 * 3 = {}\n", mult(5));

    std::cout << "\n--- for_each with Counter ---\n";
    Counter algoCounter;
    std::ranges::for_each(values, [&](int) { algoCounter(); });

    const int total = std::reduce(values.begin(), values.end(), 0, std::plus<>{});
    std::cout << std::format("\nTotal sum: {}\n", total);

    std::cout << std::format("Count > 12: {}\n",
                             std::ranges::count_if(values, GreaterThan(12)));

    std::cout << "\n--- Applying multiplier to all ---\n";
    for (int v : transform_copy(values, mult))
        std::cout << std::format("{} ", v);
    std::cout << '\n';

    std::cout << "\n--- Extra Functors ---\n";
    std::cout << std::format("Accumulated subtraction: {}\n",
                             std::accumulate(values.begin(), values.end(), 0, std::minus<>{}));
    std::cout << std::format("Even count: {}\n",
                             std::ranges::count_if(values, IsEven{}));

    std::cout << "Values: ";
    std::ranges::for_each(values, [](int x) { std::cout << std::format("{} ", x); });
    std::cout << '\n';

    std::cout << "\n--- Advanced Callable Objects ---\n";
    const auto squared = transform_copy(values, Square{});
    std::cout << "Squared: ";
    for (int x : squared) std::cout << std::format("{} ", x);
    std::cout << '\n';

    std::cout << std::format("Values in [12,14]: {}\n",
                             std::ranges::count_if(values, InRange(12, 14)));

    std::cout << "\nFormatted output:\n";
    std::ranges::for_each(values, Formatter("Value = "));

    std::cout << std::format("\nExecute Adder(20) on 5:      {}\n", execute(Adder(20), 5));
    std::cout << std::format("Execute Multiplier(4) on 3:  {}\n", execute(Multiplier<int>(4), 3));
    std::cout << std::format("Execute Square on 7:         {}\n", execute(Square{}, 7));

    std::function<int(int)> callable = Multiplier<int>(5);
    std::cout << std::format("std::function result:        {}\n", callable(6));
    std::cout << std::format("Counter final state:         {}\n", counter.get_count());

    std::cout << "\n--- Compose ---\n";
    auto add_then_square = compose(Square{}, Adder(3));
    std::cout << std::format("compose(Square, Adder(3))(4) = {}\n", add_then_square(4));

    auto square_then_add = compose(Adder(3), Square{});
    std::cout << std::format("compose(Adder(3), Square)(4) = {}\n", square_then_add(4));

    std::cout << "\n--- Negated & Both ---\n";
    Negated not_even{IsEven{}};
    std::cout << std::format("Odd count: {}\n",
                             std::ranges::count_if(values, not_even));

    Both in_range_and_even{InRange(10, 15), IsEven{}};
    std::cout << std::format("In [10,15] and even: {}\n",
                             std::ranges::count_if(values, in_range_and_even));

    std::cout << "\n--- Clamp ---\n";
    const auto clamped = transform_copy(values, Clamp(12, 14));
    std::cout << "Clamped to [12,14]: ";
    for (int v : clamped) std::cout << std::format("{} ", v);
    std::cout << '\n';

    std::cout << "\n--- Adder composition ---\n";
    const auto adder_sum = Adder(3) + Adder(7);
    std::cout << std::format("Adder(3) + Adder(7) applied to 0: {}\n", adder_sum(0));

    std::cout << "\n--- views pipeline with functors ---\n";
    auto pipeline = values
        | std::views::filter(InRange(10, 15))
        | std::views::transform(Square{})
        | std::views::take(3);
    std::cout << "filter(10-15) | square | take(3): ";
    for (int v : pipeline) std::cout << std::format("{} ", v);
    std::cout << '\n';

    assert(mult(2) == 6);
    assert(execute(Adder(10), 5) == 15);
    assert(InRange(1, 10)(5));
    assert(!IsEven{}(3));
    assert(add_then_square(4) == (4 + 3) * (4 + 3));
    assert(square_then_add(4) == 4 * 4 + 3);
    assert(Clamp(0, 10)(15) == 10);
    assert(Clamp(0, 10)(-5) == 0);
    assert(not_even(3));
    assert(!not_even(4));
    assert(adder_sum(0) == 10);
    assert(find_first(values, GreaterThan(100)) == std::nullopt);

    std::cout << "\nAll assertions passed.\n";
    return 0;
}
