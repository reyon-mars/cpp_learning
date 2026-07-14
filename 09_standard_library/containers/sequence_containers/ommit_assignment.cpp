#include <iostream>
#include <list>
#include <numeric>
#include <algorithm>
#include <optional>
#include <vector>
#include <ranges>
#include <functional>
#include <cassert>
#include <string_view>
#include <cstddef>

void print_list(const std::list<int>& lst, std::string_view label = "") {
    if (!label.empty()) std::cout << label << ": ";
    for (int x : lst) std::cout << x << " ";
    std::cout << "\n";
}

[[nodiscard]] int sum_list(const std::list<int>& lst) noexcept {
    return std::accumulate(lst.cbegin(), lst.cend(), 0);
}

[[nodiscard]] int product_list(const std::list<int>& lst) noexcept {
    return lst.empty() ? 0
                       : std::accumulate(lst.cbegin(), lst.cend(), 1, std::multiplies<>{});
}

[[nodiscard]] std::optional<int> max_list(const std::list<int>& lst) {
    if (lst.empty()) return std::nullopt;
    return *std::ranges::max_element(lst);
}

[[nodiscard]] std::optional<int> min_list(const std::list<int>& lst) {
    if (lst.empty()) return std::nullopt;
    return *std::ranges::min_element(lst);
}

[[nodiscard]] std::optional<double> average_list(const std::list<int>& lst) {
    if (lst.empty()) return std::nullopt;
    return static_cast<double>(sum_list(lst)) / static_cast<double>(lst.size());
}

[[nodiscard]] bool contains_value(const std::list<int>& lst, int value) {
    return std::ranges::find(lst, value) != lst.end();
}

[[nodiscard]] long count_value(const std::list<int>& lst, int value) {
    return std::ranges::count(lst, value);
}

[[nodiscard]] bool all_positive(const std::list<int>& lst) {
    return std::ranges::all_of(lst, [](int v) { return v > 0; });
}

[[nodiscard]] bool all_even(const std::list<int>& lst) {
    return std::ranges::all_of(lst, [](int v) { return v % 2 == 0; });
}

[[nodiscard]] long count_if_list(const std::list<int>& lst, std::predicate<int> auto pred) {
    return std::ranges::count_if(lst, pred);
}

[[nodiscard]] std::vector<int> to_vector(const std::list<int>& lst) {
    return {lst.begin(), lst.end()};
}

[[nodiscard]] std::list<int> from_vector(const std::vector<int>& v) {
    return {v.begin(), v.end()};
}

[[nodiscard]] std::list<int> filter_list(const std::list<int>& lst,
                                          std::predicate<int> auto pred) {
    std::list<int> result;
    std::ranges::copy_if(lst, std::back_inserter(result), pred);
    return result;
}

[[nodiscard]] std::list<int> transform_list(const std::list<int>& lst,
                                             std::invocable<int> auto fn) {
    std::list<int> result;
    std::ranges::transform(lst, std::back_inserter(result), fn);
    return result;
}

[[nodiscard]] std::list<int> merge_sorted(std::list<int> a, std::list<int> b) {
    a.sort(); b.sort();
    a.merge(b);
    return a;
}

[[nodiscard]] std::optional<int> first_match(const std::list<int>& lst,
                                              std::predicate<int> auto pred) {
    auto it = std::ranges::find_if(lst, pred);
    return it != lst.end() ? std::optional{*it} : std::nullopt;
}

int main() {
    std::list<int> v{1, 2, 3};
    auto it = v.begin();
    while (it != v.end()) {
        ++it;
        it = v.insert(it, 42);
        ++it;
    }
    print_list(v, "After insert");

    std::cout << "size="    << v.size()                         << "\n"
              << "empty="   << std::boolalpha << v.empty()      << "\n"
              << "sum="     << sum_list(v)                      << "\n"
              << "product=" << product_list(v)                  << "\n";
    if (auto m = max_list(v))     std::cout << "max="  << *m << "\n";
    if (auto m = min_list(v))     std::cout << "min="  << *m << "\n";
    if (auto a = average_list(v)) std::cout << "avg="  << *a << "\n";

    std::cout << "contains(42)=" << contains_value(v, 42) << "\n"
              << "count(42)="    << count_value(v, 42)    << "\n"
              << "all_positive=" << all_positive(v)       << "\n"
              << "count_gt_5="   << count_if_list(v, [](int x) { return x > 5; }) << "\n";

    if (auto f = first_match(v, [](int x) { return x > 5; }))
        std::cout << "first>5=" << *f << "\n";

    std::cout << "\n=== reverse / sort / remove / unique ===\n";
    v.reverse();           print_list(v, "Reversed");
    v.sort();              print_list(v, "Sorted");
    v.remove(42);          print_list(v, "After remove(42)");
    v.push_back(1); v.push_back(1);
    v.unique();            print_list(v, "After unique");

    std::cout << "\n=== push/pop / front / back ===\n";
    v.push_front(100); v.push_back(200);
    print_list(v, "After push_front/back");
    std::cout << "front=" << v.front() << " back=" << v.back() << "\n";
    v.pop_front(); v.pop_back();
    print_list(v, "After pop_front/back");

    std::cout << "\n=== to_vector / from_vector ===\n";
    auto vec  = to_vector(v);
    auto lst2 = from_vector(vec);
    print_list(lst2, "from_vector");
    std::cout << "sizes match=" << (v.size() == lst2.size()) << "\n";

    std::cout << "\n=== filter_list(>1) ===\n";
    print_list(filter_list(v, [](int x) { return x > 1; }), "filtered");

    std::cout << "\n=== transform_list(x*2) ===\n";
    print_list(transform_list(v, [](int x) { return x * 2; }), "doubled");

    std::cout << "\n=== merge_sorted ===\n";
    print_list(merge_sorted({5, 1, 3}, {6, 2, 4}), "merged");

    std::cout << "\n=== splice ===\n";
    std::list<int> extra{97, 98, 99};
    v.splice(v.end(), extra);
    print_list(v, "After splice");
    std::cout << "extra empty=" << extra.empty() << "\n";

    std::cout << "\n=== remove_if ===\n";
    v.remove_if([](int x) { return x % 2 != 0; });
    print_list(v, "After remove_if(odd)");
    std::cout << "all_even=" << all_even(v) << "\n";

    std::cout << "\n=== resize / clear ===\n";
    v.resize(6, 9);
    print_list(v, "After resize(6,9)");

    std::list<int> temp = v;
    temp.clear();
    std::cout << "temp after clear: size=" << temp.size() << " empty=" << temp.empty() << "\n";

    assert(sum_list({1, 2, 3}) == 6);
    assert(product_list({1, 2, 3, 4}) == 24);
    assert(max_list({3, 1, 4, 1}).value() == 4);
    assert(!max_list({}).has_value());
    assert(std::ranges::is_sorted(merge_sorted({5, 1}, {4, 2})));
    assert(filter_list({1, 2, 3, 4}, [](int x) { return x % 2 == 0; }).size() == 2);

    std::cout << "\nAll assertions passed.\n";
    return 0;
}
