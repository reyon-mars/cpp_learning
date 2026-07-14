#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <optional>
#include <string_view>
#include <span>
#include <format>
#include <ranges>
#include <cassert>
#include <concepts>
#include <functional>

template <std::ranges::input_range R>
void print_range(const R& r, std::string_view label = "") {
    if (!label.empty()) std::cout << std::format("{}: ", label);
    for (const auto& v : r) std::cout << std::format("{} ", v);
    std::cout << '\n';
}

[[nodiscard]] int sum_vec(std::span<const int> v) noexcept {
    return std::reduce(v.begin(), v.end(), 0);
}

[[nodiscard]] std::optional<int> max_vec(std::span<const int> v) {
    if (v.empty()) return std::nullopt;
    return *std::ranges::max_element(v);
}

[[nodiscard]] std::optional<int> min_vec(std::span<const int> v) {
    if (v.empty()) return std::nullopt;
    return *std::ranges::min_element(v);
}

[[nodiscard]] std::optional<double> avg_vec(std::span<const int> v) {
    if (v.empty()) return std::nullopt;
    return static_cast<double>(sum_vec(v)) / static_cast<double>(v.size());
}

[[nodiscard]] bool contains_value(std::span<const int> v, int n) {
    return std::ranges::contains(v, n);
}

[[nodiscard]] long count_even(std::span<const int> v) {
    return std::ranges::count_if(v, [](int x) { return x % 2 == 0; });
}

[[nodiscard]] long count_odd(std::span<const int> v) {
    return std::ranges::count_if(v, [](int x) { return x % 2 != 0; });
}

[[nodiscard]] bool is_sorted_vec(std::span<const int> v) {
    return std::ranges::is_sorted(v);
}

struct VecStats {
    std::size_t size{};
    int         sum{};
    int         min{};
    int         max{};
    double      avg{};
    long        evens{};
    long        odds{};
};

[[nodiscard]] std::optional<VecStats> compute_stats(std::span<const int> v) {
    if (v.empty()) return std::nullopt;
    auto [mn, mx] = std::ranges::minmax(v);
    const int s   = sum_vec(v);
    return VecStats{
        v.size(), s, mn, mx,
        static_cast<double>(s) / static_cast<double>(v.size()),
        count_even(v), count_odd(v)
    };
}

void print_stats(const VecStats& st) {
    std::cout << std::format("size={} sum={} min={} max={} avg={:.2f} even={} odd={}\n",
                             st.size, st.sum, st.min, st.max, st.avg, st.evens, st.odds);
}

void print_info(std::span<const int> v) {
    if (const auto st = compute_stats(v))
        print_stats(*st);
    else
        std::cout << "empty\n";
}

template <std::predicate<int> Pred>
[[nodiscard]] std::vector<int> filter_copy(std::span<const int> v, Pred p) {
    std::vector<int> result;
    std::ranges::copy_if(v, std::back_inserter(result), p);
    return result;
}

template <std::invocable<int> Fn>
[[nodiscard]] std::vector<int> transform_copy(std::span<const int> v, Fn fn) {
    std::vector<int> result(v.size());
    std::ranges::transform(v, result.begin(), fn);
    return result;
}

[[nodiscard]] std::pair<std::vector<int>, std::vector<int>>
partition_copy(std::span<const int> v, std::predicate<int> auto p) {
    std::vector<int> yes, no;
    for (int x : v) (std::invoke(p, x) ? yes : no).push_back(x);
    return {std::move(yes), std::move(no)};
}

template <std::ranges::input_range R>
[[nodiscard]] int range_sum(const R& r) {
    using T = std::ranges::range_value_t<R>;
    return static_cast<int>(std::reduce(std::ranges::begin(r), std::ranges::end(r), T{}));
}

void extra_demo() {
    std::cout << "\n=== extra_demo ===\n";
    std::vector<int> v{1, 2, 3, 4, 5};
    print_range(v, "Initial");

    std::cout << std::format("sum={}\ncontains(3)={}\n",
                             sum_vec(v), contains_value(v, 3));

    if (auto m = max_vec(v)) std::cout << std::format("max={}\n", *m);
    if (auto m = min_vec(v)) std::cout << std::format("min={}\n", *m);
    if (auto a = avg_vec(v)) std::cout << std::format("avg={:.2f}\n", *a);

    std::cout << std::format("even={} odd={}\n", count_even(v), count_odd(v));

    std::ranges::reverse(v);
    print_range(v, "Reversed");

    std::ranges::sort(v);
    print_range(v, "Sorted");
    print_info(v);

    v.push_back(10); v.push_back(20);
    print_range(v, "After push_back");
    v.pop_back();
    print_range(v, "After pop_back");

    std::cout << std::format("sorted={}\n", is_sorted_vec(v));

    const auto evens = filter_copy(v, [](int x) { return x % 2 == 0; });
    print_range(evens, "filter_copy (evens)");

    const auto squares = transform_copy(v, [](int x) { return x * x; });
    print_range(squares, "transform_copy (squares)");

    auto [even_part, odd_part] = partition_copy(v, [](int x) { return x % 2 == 0; });
    print_range(even_part, "partition (evens)");
    print_range(odd_part,  "partition (odds)");

    auto pipeline = v
        | std::views::filter([](int x) { return x % 2 != 0; })
        | std::views::transform([](int x) { return x * x; });
    print_range(pipeline, "odd squares (lazy)");
    std::cout << std::format("sum of odd squares: {}\n", range_sum(pipeline));
}

int main() {
    std::vector<int> vec;
    vec.reserve(28);
    for (int i : std::views::iota(0, 28)) vec.push_back(i);

    std::cout << std::format("capacity={} size={}\n", vec.capacity(), vec.size());

    std::ranges::reverse(vec);
    print_range(vec, "Reversed");

    std::cout << "\n=== Info after reverse ===\n";
    print_info(vec);
    std::cout << std::format("sorted={}\n", is_sorted_vec(vec));

    std::ranges::sort(vec);
    print_range(vec, "Sorted");
    std::cout << std::format("front={} back={}\n", vec.front(), vec.back());

    extra_demo();

    std::cout << "\n=== erase-remove idiom ===\n";
    std::vector<int> er{1, 2, 3, 4, 5, 6, 7, 8};
    const auto [removed, new_end] = std::ranges::remove_if(er, [](int x) { return x % 2 == 0; });
    er.erase(removed, er.end());
    print_range(er, "After erase-remove (odds only)");
    assert(std::ranges::all_of(er, [](int x) { return x % 2 != 0; }));

    std::cout << "\n=== rotate ===\n";
    std::vector<int> rot{1, 2, 3, 4, 5};
    std::ranges::rotate(rot, rot.begin() + 2);
    print_range(rot, "Rotated by 2");

    std::cout << "\n=== adjacent views ===\n";
    std::vector<int> adj{1, 3, 6, 10, 15};
    auto diffs = adj | std::views::adjacent_transform<2>([](int a, int b) { return b - a; });
    print_range(diffs, "Adjacent differences (views)");

    std::cout << "\n=== chunk view ===\n";
    std::vector<int> chunked(10);
    std::iota(chunked.begin(), chunked.end(), 1);
    for (auto chunk : chunked | std::views::chunk(3)) {
        std::cout << std::format("  chunk sum={}\n", range_sum(chunk));
    }

    assert(sum_vec({1, 2, 3, 4, 5}) == 15);
    assert(max_vec({1, 2, 3}) == 3);
    assert(min_vec({1, 2, 3}) == 1);
    assert(!max_vec({}).has_value());
    assert(contains_value({1, 2, 3}, 2));
    assert(!contains_value({1, 2, 3}, 9));

    std::cout << "\nAll assertions passed.\n";
    return 0;
}
