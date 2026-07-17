#include <iostream>
#include <tuple>
#include <optional>
#include <variant>
#include <any>
#include <string>
#include <string_view>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ranges>
#include <format>
#include <type_traits>

template <typename... Ts>
struct overload : Ts... { using Ts::operator()...; };

int main() {
    auto p = std::make_pair(42, std::string{"answer"});
    std::cout << std::format("Pair: {}, {}\n", p.first, p.second);

    auto t = std::make_tuple(1, std::string_view{"hello"}, 3.14);
    auto& [ti, ts, td] = t;
    std::cout << std::format("Tuple: {}, {}, {}\n", ti, ts, td);

    std::optional<int> opt{10};
    std::cout << std::format("Optional value: {}\n", opt.value());

    std::variant<int, std::string> v{std::string{"hello"}};
    std::visit(overload{
        [](int n)               { std::cout << std::format("Variant int: {}\n", n);    },
        [](const std::string& s){ std::cout << std::format("Variant string: {}\n", s); },
    }, v);

    std::any a{42};
    std::cout << std::format("Any value: {}\n", std::any_cast<int>(a));

    auto p2 = std::make_pair(7, std::string_view{"days"});
    std::cout << std::format("Pair2: {}, {}\n", p2.first, p2.second);

    std::cout << std::format("Tuple size: {}\n",
                             std::tuple_size_v<decltype(t)>);

    opt.reset();
    std::cout << std::format("Optional has value? {}\n", opt.has_value() ? "Yes" : "No");

    v = 100;
    std::visit(overload{
        [](int n)               { std::cout << std::format("Variant int: {}\n", n);    },
        [](const std::string& s){ std::cout << std::format("Variant string: {}\n", s); },
    }, v);

    a = std::string{"stored string"};
    if (a.type() == typeid(std::string))
        std::cout << std::format("Any string: {}\n", std::any_cast<const std::string&>(a));

    auto [num, text] = p;
    std::cout << std::format("Structured binding: {}, {}\n", num, text);

    std::optional<int> empty_opt;
    std::cout << std::format("Optional value_or: {}\n", empty_opt.value_or(999));

    std::cout << std::format("Variant index: {}\n", v.index());

    std::cout << std::format("Any has value: {}\n", a.has_value() ? "Yes" : "No");

    auto p3 = std::make_pair(1, std::string{"one"});
    std::swap(p, p3);
    std::cout << std::format("After swap, p: {}, {}\n", p.first, p.second);

    opt.emplace(55);
    std::cout << std::format("Optional after emplace: {}\n", opt.value());

    v = std::string{"new variant text"};
    std::visit(overload{
        [](int n)               { std::cout << std::format("Variant: {}\n", n);    },
        [](const std::string& s){ std::cout << std::format("Variant: {}\n", s); },
    }, v);

    a.reset();
    std::cout << std::format("Any after reset has value? {}\n", a.has_value() ? "Yes" : "No");

    const std::vector<std::pair<int, std::string>> vp{
        {1, "one"}, {2, "two"}, {3, "three"}
    };

    std::cout << "Vector of pairs:\n";
    for (const auto& [id, word] : vp)
        std::cout << std::format("  {} -> {}\n", id, word);

    constexpr std::array nums{1, 2, 3, 4, 5};

    std::cout << std::format("Accumulated sum: {}\n",
                             std::reduce(nums.begin(), nums.end(), 0));

    std::cout << std::format("Even count: {}\n",
                             std::ranges::count_if(nums, [](int x){ return x % 2 == 0; }));

    const auto [min_it, max_it] = std::ranges::minmax_element(nums);
    std::cout << std::format("Min: {}  Max: {}\n", *min_it, *max_it);

    std::optional<int> opt_a{5}, opt_b{10};
    std::cout << std::format("opt_a < opt_b? {}\n", opt_a < opt_b ? "Yes" : "No");

    a = 3.1415;
    if (a.type() == typeid(double))
        std::cout << std::format("Any double: {}\n", std::any_cast<double>(a));

    std::vector<std::optional<int>> sparse{1, std::nullopt, 3, std::nullopt, 5};
    const auto present_sum = std::accumulate(sparse.begin(), sparse.end(), 0,
        [](int acc, const std::optional<int>& o){ return acc + o.value_or(0); });
    std::cout << std::format("Sum of present optionals: {}\n", present_sum);

    using Record = std::tuple<int, std::string, double>;
    const std::vector<Record> records{
        {1, "alpha", 1.1}, {2, "beta", 2.2}, {3, "gamma", 3.3}
    };
    std::cout << "Records:\n";
    for (const auto& [id, name, score] : records)
        std::cout << std::format("  id={} name={} score={}\n", id, name, score);

    return 0;
}
//next
