#include <iostream>
#include <vector>
#include <numeric>
#include <utility>
#include <memory>
#include <algorithm>
#include <functional>
#include <chrono>
#include <cassert>
#include <string>
#include <string_view>
#include <ranges>
#include <span>

[[nodiscard]] auto sum_vector(std::vector<int> vec) {
    return [v = std::move(vec)]() mutable {
        int sum = std::accumulate(v.begin(), v.end(), 0);
        v.clear();
        return sum;
    };
}

void print_vector(std::span<const int> vec, std::string_view label) {
    std::cout << label << ": ";
    for (int n : vec) std::cout << n << ' ';
    std::cout << '\n';
}

[[nodiscard]] double average_vector(std::span<const int> vec) {
    if (vec.empty()) return 0.0;
    return static_cast<double>(std::accumulate(vec.begin(), vec.end(), 0)) / vec.size();
}

[[nodiscard]] int max_element_value(std::span<const int> vec) {
    return *std::ranges::max_element(vec);
}

template<std::invocable Func>
void measure(std::string_view label, Func&& func) {
    auto start = std::chrono::steady_clock::now();
    std::invoke(std::forward<Func>(func));
    auto elapsed = std::chrono::steady_clock::now() - start;
    std::cout << label << ": "
              << std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count()
              << " us\n";
}

int main() {
    const std::vector<int> vec = {1, 2, 3, 4, 5};

    const int x = [&vec]() {
        return std::accumulate(vec.begin(), vec.end(), 0);
    }();
    std::cout << "IILE result: " << x << '\n';

    auto fn = sum_vector(vec);
    std::cout << "First sum_vector call:  " << fn() << '\n';
    std::cout << "Second sum_vector call: " << fn() << '\n';

    constexpr auto square   = [](int n) noexcept { return n * n;      };
    constexpr auto is_even  = [](int n) noexcept { return n % 2 == 0; };

    std::cout << "Square of 6 = " << square(6) << '\n';
    std::cout << "Is 10 even?  " << (is_even(10) ? "Yes" : "No") << '\n';

    std::vector<int> doubled(vec.size());
    std::ranges::transform(vec, doubled.begin(), [](int n) noexcept { return n * 2; });
    print_vector(doubled, "Doubled");

    std::cout << "Even count in doubled: "
              << std::ranges::count_if(doubled, is_even) << '\n';

    std::cout << "Original vector size: " << vec.size() << '\n';

    auto counter = [count = 0]() mutable { return ++count; };
    std::cout << "Counter: " << counter() << ", " << counter() << ", " << counter() << '\n';

    auto ptr_lambda = [ptr = std::make_unique<int>(42)]() { return *ptr; };
    std::cout << "Moved unique_ptr value: " << ptr_lambda() << '\n';

    auto add = [](auto a, auto b) { return a + b; };
    std::cout << "Generic add: " << add(3, 4.5) << '\n';

    auto make_multiplier = [](int factor) noexcept {
        return [factor](int x) noexcept { return x * factor; };
    };
    auto triple = make_multiplier(3);
    std::cout << "Triple of 7: " << triple(7) << '\n';

    const int total = std::accumulate(vec.begin(), vec.end(), 0);
    std::cout << "Sum: " << total << '\n';

    auto evens_view = vec | std::views::filter(is_even);
    std::vector<int> evens(evens_view.begin(), evens_view.end());
    print_vector(evens, "Filtered (even only)");

    auto sorted = vec;
    std::ranges::sort(sorted, std::greater<>{});
    print_vector(sorted, "Sorted descending");

    std::cout << "All even? " << (std::ranges::all_of(vec, is_even) ? "Yes" : "No") << '\n';
    std::cout << "Any even? " << (std::ranges::any_of(vec, is_even) ? "Yes" : "No") << '\n';

    const int weighted = std::accumulate(vec.begin(), vec.end(), 0,
        [](int acc, int val) noexcept { return acc + val * 2; });
    std::cout << "Weighted sum (x2): " << weighted << '\n';

    std::cout << "\n--- Advanced Lambda Features ---\n";

    auto factorial = [](auto self, int n) -> int {
        return (n <= 1) ? 1 : n * self(self, n - 1);
    };
    std::cout << "Factorial(5): " << factorial(factorial, 5) << '\n';

    const int base = 10;
    auto complex_capture = [b = base, &vec]() { return b + static_cast<int>(vec.size()); };
    std::cout << "Init + ref capture: " << complex_capture() << '\n';

    std::vector<std::pair<int,int>> pairs = {{1,2},{3,1},{2,5}};
    std::ranges::sort(pairs, [](const auto& a, const auto& b) {
        return a.second < b.second;
    });
    std::cout << "Sorted pairs by second: ";
    for (const auto& [fst, snd] : pairs) std::cout << '(' << fst << ',' << snd << ") ";
    std::cout << '\n';

    auto lazy = [&vec]() { return std::accumulate(vec.begin(), vec.end(), 0); };
    std::cout << "Lazy execution result: " << lazy() << '\n';

    measure("accumulate", [&] { std::accumulate(vec.begin(), vec.end(), 0); });

    std::cout << "\n--- Extra Lambda Utilities ---\n";

    print_vector(vec, "Original vector");
    std::cout << "Average value: " << average_vector(vec) << '\n';
    std::cout << "Maximum value: " << max_element_value(vec) << '\n';

    constexpr int captured = 50;
    auto capture_demo = [captured]() noexcept { return captured + 25; };
    std::cout << "Capture demo: " << capture_demo() << '\n';

    auto mutable_lambda = [num = 0]() mutable { num += 5; return num; };
    std::cout << "Mutable lambda: " << mutable_lambda() << ", " << mutable_lambda() << '\n';

    auto concat = [](const auto& a, const auto& b) { return a + b; };
    std::cout << "Concatenated: " << concat(std::string("Hello "), std::string("World")) << '\n';

    assert(square(5) == 25);
    assert(factorial(factorial, 5) == 120);
    assert(average_vector(vec) == 3.0);

    auto power_of = [](int power) noexcept {
        return [power](int value) noexcept {
            int result = 1;
            for (int i = 0; i < power; ++i) result *= value;
            return result;
        };
    };
    auto cube = power_of(3);
    std::cout << "Cube of 4: " << cube(4) << '\n';

    std::cout << "\nAll assertions passed.\n";
    return 0;
}

