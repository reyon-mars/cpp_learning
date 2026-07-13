#include <list>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <string_view>
#include <optional>
#include <format>
#include <ranges>
#include <vector>
#include <cassert>
#include <concepts>
#include <functional>

template <std::ranges::input_range R>
void print_range(const R& r, std::string_view label = "") {
    if (!label.empty()) std::cout << std::format("{}: ", label);
    for (const auto& v : r) std::cout << std::format("{} ", v);
    std::cout << '\n';
}

void print_reverse(const std::list<int>& L) {
    print_range(L | std::views::reverse, "Reversed view");
}

[[nodiscard]] std::size_t count_odds(const std::list<int>& L) {
    return static_cast<std::size_t>(std::ranges::count_if(L, [](int v) { return v % 2 != 0; }));
}

[[nodiscard]] std::size_t count_evens(const std::list<int>& L) {
    return L.size() - count_odds(L);
}

[[nodiscard]] int sum_list(const std::list<int>& L) noexcept {
    return std::reduce(L.cbegin(), L.cend(), 0);
}

[[nodiscard]] std::optional<int> max_list(const std::list<int>& L) {
    if (L.empty()) return std::nullopt;
    return *std::ranges::max_element(L);
}

[[nodiscard]] std::optional<int> min_list(const std::list<int>& L) {
    if (L.empty()) return std::nullopt;
    return *std::ranges::min_element(L);
}

[[nodiscard]] std::optional<double> average_list(const std::list<int>& L) {
    if (L.empty()) return std::nullopt;
    return static_cast<double>(sum_list(L)) / static_cast<double>(L.size());
}

template <std::predicate<int> Pred>
[[nodiscard]] std::list<int> filter_copy(const std::list<int>& L, Pred p) {
    std::list<int> result;
    std::ranges::copy_if(L, std::back_inserter(result), p);
    return result;
}

template <std::invocable<int> Fn>
[[nodiscard]] std::list<int> transform_copy(const std::list<int>& L, Fn fn) {
    std::list<int> result;
    std::ranges::transform(L, std::back_inserter(result), fn);
    return result;
}

[[nodiscard]] std::vector<int> to_vector(const std::list<int>& L) {
    return {L.begin(), L.end()};
}

[[nodiscard]] std::list<int> from_vector(const std::vector<int>& v) {
    return {v.begin(), v.end()};
}

void process_list() {
    std::list<int> lst{1, 2, 3, 4, 5, 6, 7, 8, 9};
    auto it = lst.begin();
    while (it != lst.end()) {
        if (*it % 2) {
            it = lst.insert(it, *it);
            std::advance(it, 2);
        } else {
            it = lst.erase(it);
        }
    }
    print_range(lst, "Processed");
    std::cout << std::format("size={} front={} back={}\n",
                             lst.size(), lst.front(), lst.back());
}

int main() {
    std::cout << "=== process_list (duplicate odds, erase evens) ===\n";
    process_list();

    std::cout << "\n=== demo list ===\n";
    std::list<int> demo{2, 4, 6, 8};
    print_range(demo, "Initial");

    std::cout << std::format("size={}\nempty={}\nodds={}\nevens={}\nsum={}\n",
                             demo.size(), demo.empty(),
                             count_odds(demo), count_evens(demo),
                             sum_list(demo));

    if (auto m = max_list(demo))     std::cout << std::format("max={}\n", *m);
    if (auto m = min_list(demo))     std::cout << std::format("min={}\n", *m);
    if (auto a = average_list(demo)) std::cout << std::format("avg={:.2f}\n", *a);

    std::cout << "\n=== reverse / sort ===\n";
    demo.reverse();
    print_range(demo, "Reversed");
    demo.sort();
    print_range(demo, "Sorted");

    std::cout << "\n=== sort with custom comparator ===\n";
    demo.sort(std::greater<>{});
    print_range(demo, "Sorted descending");
    demo.sort();

    std::cout << "\n=== remove / unique ===\n";
    demo.remove(4);
    print_range(demo, "After remove(4)");
    demo.push_back(6); demo.push_back(6);
    demo.unique();
    print_range(demo, "After unique");

    std::cout << "\n=== unique with custom predicate ===\n";
    std::list<int> close_vals{1, 2, 4, 5, 7, 8};
    close_vals.unique([](int a, int b) { return std::abs(b - a) <= 1; });
    print_range(close_vals, "unique (diff<=1)");

    std::cout << "\n=== merge ===\n";
    std::list<int> other{1, 3, 5};
    other.sort();
    demo.merge(other);
    print_range(demo, "After merge");
    std::cout << std::format("other empty={}\n", other.empty());

    std::cout << "\n=== splice ===\n";
    std::list<int> extra{100, 200};
    demo.splice(demo.begin(), extra);
    print_range(demo, "After splice");
    std::cout << std::format("extra empty={}\n", extra.empty());

    std::cout << "\n=== remove_if (odd) ===\n";
    demo.remove_if([](int x) { return x % 2 != 0; });
    print_range(demo, "After remove_if");
    std::cout << std::format("empty={}\n", demo.empty());
    if (auto m = min_list(demo))     std::cout << std::format("min={}\n", *m);
    if (auto a = average_list(demo)) std::cout << std::format("avg={:.2f}\n", *a);

    std::cout << "\n=== reverse print ===\n";
    print_reverse(demo);

    std::cout << "\n=== push_front / push_back / resize / clear ===\n";
    demo.push_front(500);
    demo.push_back(900);
    print_range(demo, "After push");
    demo.resize(5);
    print_range(demo, "After resize(5)");
    demo.clear();
    std::cout << std::format("After clear size={}\n", demo.size());

    std::cout << "\n=== filter_copy / transform_copy ===\n";
    std::list<int> base{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    const auto evens = filter_copy(base, [](int x) { return x % 2 == 0; });
    print_range(evens, "filter_copy (evens)");

    const auto squares = transform_copy(base, [](int x) { return x * x; });
    print_range(squares, "transform_copy (squares)");

    std::cout << "\n=== to_vector / from_vector round-trip ===\n";
    const auto vec = to_vector(base);
    const auto back = from_vector(vec);
    assert(std::ranges::equal(base, back));
    std::cout << "round-trip verified\n";

    std::cout << "\n=== views pipeline over list ===\n";
    auto pipeline = base
        | std::views::filter([](int x) { return x % 2 == 0; })
        | std::views::transform([](int x) { return x * x; });
    print_range(pipeline, "even squares (lazy)");

    std::cout << "\n=== splice single element ===\n";
    std::list<int> src{10, 20, 30};
    std::list<int> dst{1, 2, 3};
    auto splice_it = std::next(src.begin());
    dst.splice(dst.end(), src, splice_it);
    print_range(src, "src after single-element splice");
    print_range(dst, "dst after single-element splice");

    std::cout << "\n=== move semantics ===\n";
    std::list<int> moved = std::move(base);
    std::cout << std::format("base size after move={}\n", base.size());
    std::cout << std::format("moved size={}\n", moved.size());

    assert(count_evens(evens) == evens.size());
    assert(sum_list(evens) == 2+4+6+8+10);
    assert(max_list(evens) == 10);
    assert(min_list(evens) == 2);
    assert(!max_list({}).has_value());

    std::cout << "\nAll assertions passed.\n";
    return 0;
}
