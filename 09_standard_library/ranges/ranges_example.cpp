#include <iostream>
#include <vector>
#include <ranges>
#include <numeric>
#include <algorithm>
#include <string_view>

template<std::ranges::input_range Range>
void print_range(Range&& r, std::string_view label = "") {
    if (!label.empty()) std::cout << label << ": ";
    for (auto v : r) std::cout << v << " ";
    std::cout << "\n";
}

template<std::ranges::input_range Range>
[[nodiscard]] auto range_sum(Range&& r) {
    using T = std::ranges::range_value_t<std::remove_cvref_t<Range>>;
    return std::accumulate(std::ranges::begin(r), std::ranges::end(r), T{});
}

int main() {
    const std::vector<int> vec{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    std::cout << "=== Filter / transform ===\n";
    print_range(vec | std::views::filter([](int x){ return x % 2 == 0; }), "evens");
    print_range(vec | std::views::filter([](int x){ return x % 2 != 0; }), "odds");
    print_range(vec | std::views::transform([](int x){ return x * 2;     }), "doubled");
    print_range(vec | std::views::transform([](int x){ return x * x * x; }), "cubed");

    std::cout << "\n=== Slicing ===\n";
    print_range(vec | std::views::take(5),    "first 5");
    print_range(vec | std::views::drop(5),    "after 5");
    print_range(vec | std::views::reverse,    "reversed");
    print_range(vec | std::views::reverse | std::views::take(3), "last 3 reversed");

    std::cout << "\n=== Chained views ===\n";
    print_range(
        vec | std::views::filter([](int x){ return x % 2 == 0; })
            | std::views::transform([](int x){ return x * x; }),
        "even squared");
    print_range(
        vec | std::views::filter([](int x){ return x > 5; }),
        "greater than 5");

    std::cout << "\n=== Algorithms ===\n";
    std::cout << std::boolalpha
              << "size="        << std::ranges::distance(vec)                                   << "\n"
              << "sum="         << range_sum(vec)                                               << "\n"
              << "all_positive="<< std::ranges::all_of(vec, [](int x){ return x > 0; })         << "\n"
              << "is_sorted="   << std::ranges::is_sorted(vec)                                  << "\n"
              << "count_even="  << std::ranges::count_if(vec, [](int x){ return x % 2 == 0; }) << "\n"
              << "count_odd="   << std::ranges::count_if(vec, [](int x){ return x % 2 != 0; }) << "\n";

    const auto [lo, hi] = std::ranges::minmax(vec);
    std::cout << "min=" << lo << " max=" << hi << "\n";

    if (auto it = std::ranges::find_if(vec, [](int x){ return x > 5; }); it != vec.end())
        std::cout << "first>5=" << *it << "\n";

    std::cout << "\n=== Materialise view into vector ===\n";
    auto evens_view = vec | std::views::filter([](int x){ return x % 2 == 0; });
    std::vector<int> even_vec;
    std::ranges::copy(evens_view, std::back_inserter(even_vec));
    print_range(even_vec, "copied evens");
    std::cout << "sum of evens=" << range_sum(even_vec) << "\n";

    return 0;
}
