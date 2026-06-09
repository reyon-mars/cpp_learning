#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <functional>
#include <ranges>
#include <span>
#include <string_view>
#include <cassert>
#include <utility>
#include <concepts>
#include <stdexcept>

[[nodiscard]] int add(int a, int b)      noexcept { return a + b;     }
[[nodiscard]] int multiply(int a, int b) noexcept { return a * b;     }

template<typename T, std::invocable<T, T> BinOp>
[[nodiscard]] T reduce(std::span<const T> values, BinOp op, T init) {
    return std::accumulate(values.begin(), values.end(), init, std::move(op));
}

template<typename T, std::invocable<T, T> BinOp>
[[nodiscard]] T safe_reduce(std::span<const T> values, BinOp op, T init = T{}) {
    if (values.empty()) return init;
    return reduce(values, std::move(op), init);
}

[[nodiscard]] auto make_multiplier(int factor) {
    return [factor](int x) noexcept { return x * factor; };
}

template<typename T, std::invocable<T> Func>
[[nodiscard]] std::vector<T> map(std::span<const T> values, Func f) {
    std::vector<T> result(values.size());
    std::ranges::transform(values, result.begin(), std::move(f));
    return result;
}

template<typename T, std::predicate<T> Pred>
[[nodiscard]] std::vector<T> filter(std::span<const T> values, Pred p) {
    std::vector<T> result;
    std::ranges::copy_if(values, std::back_inserter(result), std::move(p));
    return result;
}

template<std::invocable<int> F, std::invocable<int> G>
[[nodiscard]] auto compose(F f, G g) {
    return [f = std::move(f), g = std::move(g)](auto x) {
        return std::invoke(f, std::invoke(g, x));
    };
}

template<typename T, std::invocable<T> Func>
[[nodiscard]] auto pipe(T value, Func f) {
    return std::invoke(std::move(f), std::move(value));
}

template<std::predicate<int> P1, std::predicate<int> P2>
[[nodiscard]] auto and_predicate(P1 p1, P2 p2) {
    return [p1 = std::move(p1), p2 = std::move(p2)](int x) {
        return std::invoke(p1, x) && std::invoke(p2, x);
    };
}

template<std::predicate<int> Pred>
[[nodiscard]] bool any_match(std::span<const int> values, Pred p) {
    return std::ranges::any_of(values, std::move(p));
}

template<std::invocable<int> Func>
void repeat_action(int times, Func f) {
    for (int i = 0; i < times; ++i)
        std::invoke(f, i);
}

void print_vector(std::span<const int> values, std::string_view label) {
    std::cout << label;
    for (int v : values) std::cout << v << ' ';
    std::cout << '\n';
}

int main() {
    const std::vector<int> values = {1, 2, 3, 4, 5};

    const int sum     = reduce<int>(values, add,      0);
    const int product = reduce<int>(values, multiply, 1);

    std::cout << "Sum:     " << sum     << '\n';
    std::cout << "Product: " << product << '\n';

    const int max_val = reduce<int>(values,
        [](int a, int b) noexcept { return std::max(a, b); }, values.front());
    std::cout << "Max: " << max_val << '\n';

    const std::vector<double> dbls = {1.0, 2.0, 3.0};
    const double avg = reduce<double>(dbls,
        [](double a, double b) noexcept { return a + b; }, 0.0) / 3.0;
    std::cout << "Average: " << avg << '\n';

    std::cout << "std::accumulate sum: "
              << std::accumulate(values.begin(), values.end(), 0) << '\n';

    const auto doubled = map<int>(values, make_multiplier(2));
    print_vector(doubled, "Doubled: ");

    const auto evens = filter<int>(values, [](int x) noexcept { return x % 2 == 0; });
    print_vector(evens, "Evens: ");

    auto squared_vals = map<int>(values, [](int x) noexcept { return x * x; });
    print_vector(squared_vals, "Squared: ");

    const int custom = reduce<int>(values,
        [](int a, int b) noexcept { return a + b * 2; }, 0);
    std::cout << "Custom reduce: " << custom << '\n';

    std::cout << "\n--- Advanced Functional Patterns ---\n";

    const auto tripled = map<int>(values, [](int x) noexcept { return x * 3; });
    print_vector(tripled, "Tripled: ");

    const auto double_then_square = compose(
        [](int x) noexcept { return x * x; },
        make_multiplier(2));
    std::cout << "compose(double->square)(5): " << double_then_square(5) << '\n';

    std::cout << "Pipe result: " << pipe(5, make_multiplier(3)) << '\n';

    const auto even_and_gt10 = and_predicate(
        [](int x) noexcept { return x % 2 == 0; },
        [](int x) noexcept { return x > 10; });
    print_vector(filter<int>(squared_vals, even_and_gt10), "Even and >10: ");

    const std::vector<int> empty;
    std::cout << "Safe reduce (empty): " << safe_reduce<int>(empty, add) << '\n';

    std::cout << "\n--- Extra Higher-Order Tests ---\n";

    print_vector(map<int>(values, [](int x) noexcept { return -x; }), "Negatives: ");

    repeat_action(3, [](int i) { std::cout << "Repeated call #" << (i + 1) << '\n'; });

    std::cout << "Any value > 20? "
              << (any_match(squared_vals, [](int x) noexcept { return x > 20; }) ? "Yes" : "No")
              << '\n';

    assert(sum == 15);
    assert(product == 120);
    assert(max_val == 5);
    assert(safe_reduce<int>(empty, add) == 0);
    assert(double_then_square(5) == 100);

    std::cout << "\nAll assertions passed.\n";
    return 0;
}
