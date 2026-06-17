#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <string>
#include <ranges>
#include <format>
#include <concepts>
#include <cstdlib>

template <std::ranges::input_range R>
void print_range(std::string_view label, const R& range) {
    std::cout << label << ": ";
    for (const auto& v : range) std::cout << v << ' ';
    std::cout << '\n';
}

int main() {
    const std::vector ints    {1, 2, 3, 4, 5};
    const std::vector doubles {1.1, 2.2, 3.3};

    auto print_doubled = [](auto x) { std::cout << (x * 2) << ' '; };

    std::cout << "Doubled ints: ";
    std::ranges::for_each(ints, print_doubled);
    std::cout << '\n';

    std::cout << "Doubled doubles: ";
    std::ranges::for_each(doubles, print_doubled);
    std::cout << '\n';

    auto add = []<typename T>(T a, T b) -> T { return a + b; };

    std::cout << std::format("Sum (int):    {}\n", add(5, 3));
    std::cout << std::format("Sum (double): {}\n", add(2.5, 4.1));

    auto is_greater = []<typename T>(T a, T b) -> bool { return a > b; };
    std::cout << std::format("Is 10 > 5? {}\n", is_greater(10, 5) ? "Yes" : "No");

    auto print_container = [](const std::ranges::input_range auto& r) {
        for (const auto& v : r) std::cout << v << ' ';
        std::cout << '\n';
    };

    std::cout << "Ints: ";    print_container(ints);
    std::cout << "Doubles: "; print_container(doubles);

    auto square = []<typename T>(T x) -> T { return x * x; };
    std::cout << std::format("Square of 4:   {}\n", square(4));
    std::cout << std::format("Square of 2.5: {}\n", square(2.5));

    constexpr int factor = 3;
    auto scale = [factor]<typename T>(T x) -> T {
        return x * static_cast<T>(factor);
    };
    std::cout << std::format("5 scaled by {}: {}\n", factor, scale(5));

    std::vector<int> squared(ints.size());
    std::ranges::transform(ints, squared.begin(), square);
    print_range("Squared ints", squared);

    auto is_even = [](std::integral auto x) { return x % 2 == 0; };
    auto is_odd  = [](std::integral auto x) { return x % 2 != 0; };

    std::cout << "Even numbers: ";
    for (auto x : ints | std::views::filter(is_even)) std::cout << x << ' ';
    std::cout << '\n';

    std::cout << std::format("Odd count: {}\n",
                             std::ranges::count_if(ints, is_odd));

    auto minimum = []<typename T>(T a, T b) -> T { return std::min(a, b); };
    std::cout << std::format("Minimum of 8 and 3: {}\n", minimum(8, 3));

    auto join = []<typename T>(const T& a, const T& b) { return a + b; };
    std::cout << std::format("Joined: {}\n", join(std::string{"Hello "}, std::string{"World"}));

    std::cout << std::format("Sum of ints: {}\n",
                             std::reduce(ints.begin(), ints.end(), 0));

    auto absolute = []<typename T>(T x) -> T { return x < T{} ? -x : x; };
    std::cout << std::format("Absolute of -9:  {}\n", absolute(-9));
    std::cout << std::format("Absolute of -3.7: {}\n", absolute(-3.7));

    auto multiply = []<typename T>(T a, T b) -> T { return a * b; };
    std::cout << std::format("Multiply: {}\n", multiply(2.5, 4.0));

    const auto [min_it, max_it] = std::ranges::minmax_element(ints);
    std::cout << std::format("Min int: {}  Max int: {}\n", *min_it, *max_it);

    std::cout << "Reverse ints: ";
    for (const auto v : ints | std::views::reverse) std::cout << v << ' ';
    std::cout << '\n';

    std::cout << "Evens squared: ";
    for (const auto v : ints | std::views::filter(is_even)
                              | std::views::transform(square))
        std::cout << v << ' ';
    std::cout << '\n';

    auto clamp_to = [](std::integral auto lo, std::integral auto hi) {
        return [lo, hi](std::integral auto x) { return std::clamp(x, lo, hi); };
    };
    const auto clamp_1_3 = clamp_to(1, 3);
    std::vector<int> clamped(ints.size());
    std::ranges::transform(ints, clamped.begin(), clamp_1_3);
    print_range("Clamped [1,3]", clamped);

    return 0;
}
