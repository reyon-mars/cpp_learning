#include <iostream>
#include <tuple>
#include <optional>
#include <variant>
#include <string>
#include <string_view>
#include <vector>
#include <numeric>
#include <algorithm>
#include <ranges>
#include <format>
#include <concepts>

template <std::ranges::input_range R>
void print_range(std::string_view label, const R& range) {
    std::cout << label << ": ";
    for (const auto& v : range) std::cout << v << ' ';
    std::cout << '\n';
}

template <typename... Ts>
struct overload : Ts... { using Ts::operator()...; };

[[nodiscard]] std::optional<int> safe_divide(int a, int b) noexcept {
    if (b == 0) return std::nullopt;
    return a / b;
}

[[nodiscard]] std::optional<double> safe_sqrt(double x) noexcept {
    if (x < 0.0) return std::nullopt;
    return std::sqrt(x);
}

template <std::ranges::input_range R>
    requires std::integral<std::ranges::range_value_t<R>>
[[nodiscard]] double average(const R& range) noexcept {
    const auto n = static_cast<double>(std::ranges::distance(range));
    return n == 0.0 ? 0.0
                    : std::reduce(std::ranges::begin(range),
                                  std::ranges::end(range), 0.0) / n;
}

int main() {
    auto [x, y] = std::make_tuple(10, 20);
    std::cout << std::format("x: {}, y: {}\n", x, y);

    if (const auto result = safe_divide(10, 2))
        std::cout << std::format("Result: {}\n", *result);

    if (const auto result = safe_divide(10, 0))
        std::cout << std::format("Result: {}\n", *result);
    else
        std::cout << "Division by zero\n";

    std::variant<int, std::string> v{std::string{"hello"}};
    std::visit(overload{
        [](int n)               { std::cout << std::format("Variant int: {}\n", n);    },
        [](const std::string& s){ std::cout << std::format("Variant string: {}\n", s); },
    }, v);

    const auto [a, b] = std::pair{5, 7};
    std::cout << std::format("Pair values: {}, {}\n", a, b);

    std::cout << std::format("Safe result with fallback: {}\n",
                             safe_divide(20, 4).value_or(0));

    v = 42;
    std::visit(overload{
        [](int n)               { std::cout << std::format("Variant int: {}\n", n);    },
        [](const std::string& s){ std::cout << std::format("Variant string: {}\n", s); },
    }, v);

    v = std::string{"C++17"};
    std::visit(overload{
        [](int n)               { std::cout << std::format("Variant: {}\n", n);    },
        [](const std::string& s){ std::cout << std::format("Variant: {}\n", s); },
    }, v);

    std::cout << "----------------------\n";

    std::cout << std::format("Optional has value? {}\n",
                             safe_divide(5, 1).has_value() ? "Yes" : "No");

    const std::vector nums {1, 2, 3, 4};

    std::cout << std::format("Sum:     {}\n",
                             std::reduce(nums.begin(), nums.end(), 0));
    std::cout << std::format("Average: {}\n", average(nums));
    std::cout << std::format("Variant index: {}\n", v.index());

    print_range("Vector contents", nums);

    const auto [min_it, max_it] = std::ranges::minmax_element(nums);
    std::cout << std::format("Min: {}, Max: {}\n", *min_it, *max_it);

    std::cout << std::format("Contains 3? {}\n",
                             std::ranges::contains(nums, 3) ? "Yes" : "No");

    print_range("Reversed", nums | std::views::reverse);

    std::cout << std::format("Evens: ");
    for (const auto v : nums | std::views::filter([](int x){ return x % 2 == 0; }))
        std::cout << v << ' ';
    std::cout << '\n';

    const std::vector<std::optional<int>> sparse {1, std::nullopt, 3, std::nullopt, 5};
    const auto present_sum = std::transform_reduce(
        sparse.begin(), sparse.end(),
        0, std::plus<>{},
        [](const std::optional<int>& o) { return o.value_or(0); });
    std::cout << std::format("Sparse sum (nullopt=0): {}\n", present_sum);

    using Record = std::tuple<std::string_view, int, double>;
    const std::vector<Record> records {
        {"alice", 90, 4.0}, {"bob", 75, 3.2}, {"carol", 88, 3.8}
    };
    std::cout << "Records:\n";
    for (const auto& [name, score, gpa] : records)
        std::cout << std::format("  {} score={} gpa={}\n", name, score, gpa);

    using Result = std::variant<double, std::string>;
    const auto compute = [](double x) -> Result {
        if (const auto r = safe_sqrt(x)) return *r;
        return std::format("sqrt undefined for {}", x);
    };

    for (const double val : {4.0, -1.0, 9.0}) {
        std::visit(overload{
            [&](double r)           { std::cout << std::format("sqrt({}) = {}\n", val, r);    },
            [&](const std::string& e){ std::cout << std::format("Error: {}\n", e); },
        }, compute(val));
    }

    return 0;
}
