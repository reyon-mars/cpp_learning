#include <forward_list>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <optional>
#include <vector>
#include <ranges>
#include <functional>
#include <cassert>
#include <string_view>
#include <cstddef>

void print(const std::forward_list<int>& lst, std::string_view label = "") {
    if (!label.empty()) std::cout << label << ": ";
    for (int v : lst) std::cout << v << " ";
    std::cout << "\n";
}

[[nodiscard]] std::optional<int> first_greater_than(const std::forward_list<int>& lst, int n) {
    auto it = std::ranges::find_if(lst, [n](int v) { return v > n; });
    return it != lst.end() ? std::optional{*it} : std::nullopt;
}

[[nodiscard]] std::optional<int> last_element(const std::forward_list<int>& lst) {
    if (lst.empty()) return std::nullopt;
    auto it = lst.begin();
    while (std::next(it) != lst.end()) ++it;
    return *it;
}

[[nodiscard]] std::optional<double> average(const std::forward_list<int>& lst) {
    auto n = static_cast<std::ptrdiff_t>(std::ranges::distance(lst));
    if (n == 0) return std::nullopt;
    return static_cast<double>(std::accumulate(lst.cbegin(), lst.cend(), 0)) / n;
}

[[nodiscard]] std::vector<int> to_vector(const std::forward_list<int>& lst) {
    return {lst.begin(), lst.end()};
}

[[nodiscard]] std::forward_list<int> from_vector(const std::vector<int>& v) {
    std::forward_list<int> result;
    for (auto it = v.rbegin(); it != v.rend(); ++it) result.push_front(*it);
    return result;
}

[[nodiscard]] std::forward_list<int> filter_list(const std::forward_list<int>& lst,
                                                  std::predicate<int> auto pred) {
    std::forward_list<int> result;
    auto tail = result.before_begin();
    for (int v : lst)
        if (pred(v)) tail = result.insert_after(tail, v);
    return result;
}

[[nodiscard]] std::forward_list<int> transform_list(const std::forward_list<int>& lst,
                                                     std::invocable<int> auto fn) {
    std::forward_list<int> result;
    auto tail = result.before_begin();
    for (int v : lst) tail = result.insert_after(tail, std::invoke(fn, v));
    return result;
}

[[nodiscard]] std::forward_list<int> merge_sorted(std::forward_list<int> a,
                                                    std::forward_list<int> b) {
    a.sort(); b.sort();
    a.merge(b);
    return a;
}

int main() {
    std::forward_list<int> lst{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    auto prev = lst.before_begin();
    auto curr = lst.begin();
    while (curr != lst.end()) {
        if (*curr % 2) curr = lst.erase_after(prev);
        else { prev = curr; ++curr; }
    }

    print(lst, "Evens");

    const auto n    = static_cast<int>(std::ranges::distance(lst));
    const int  sum  = std::accumulate(lst.cbegin(), lst.cend(), 0);
    const int  prod = lst.empty() ? 0
                                  : std::accumulate(lst.cbegin(), lst.cend(), 1, std::multiplies<>{});

    std::cout << "count="   << n    << "\n"
              << "sum="     << sum  << "\n"
              << "product=" << prod << "\n";

    if (!lst.empty()) {
        std::cout << "front=" << lst.front()                          << "\n"
                  << "last="  << *last_element(lst)                   << "\n"
                  << "max="   << *std::ranges::max_element(lst)       << "\n"
                  << "min="   << *std::ranges::min_element(lst)       << "\n"
                  << "avg="   << *average(lst)                        << "\n";
    } else {
        std::cout << "List is empty\n";
    }

    std::cout << std::boolalpha
              << "all_even="    << std::ranges::all_of(lst,  [](int v) { return v % 2 == 0; }) << "\n"
              << "any_gt_6="    << std::ranges::any_of(lst,  [](int v) { return v > 6;       }) << "\n"
              << "none_neg="    << std::ranges::none_of(lst, [](int v) { return v < 0;       }) << "\n"
              << "contains(6)=" << (std::ranges::find(lst, 6) != lst.end())                     << "\n"
              << "even_count="  << std::ranges::count_if(lst, [](int v) { return v % 2 == 0; }) << "\n"
              << "sorted="      << std::ranges::is_sorted(lst)                                  << "\n";

    if (auto v = first_greater_than(lst, 4)) std::cout << "first>4=" << *v << "\n";

    std::cout << "\n=== to_vector / from_vector ===\n";
    auto vec  = to_vector(lst);
    auto lst2 = from_vector(vec);
    print(lst2, "from_vector");

    std::cout << "\n=== filter_list(>4) ===\n";
    print(filter_list(lst, [](int v) { return v > 4; }), "filtered");

    std::cout << "\n=== transform_list(x*x) ===\n";
    print(transform_list(lst, [](int v) { return v * v; }), "squared");

    std::cout << "\n=== merge_sorted ===\n";
    print(merge_sorted({3, 1, 5}, {6, 2, 4}), "merged");

    std::cout << "\n=== mutations ===\n";
    lst.reverse();          print(lst, "reversed");
    lst.sort();             print(lst, "sorted");
    lst.push_front(100);    print(lst, "push_front(100)");
    lst.pop_front();        print(lst, "pop_front");
    lst.remove(4);          print(lst, "remove(4)");
    lst.push_front(2); lst.push_front(2);
    lst.unique();           print(lst, "unique");
    lst.resize(4);          print(lst, "resize(4)");

    std::cout << "\n=== splice_after ===\n";
    std::forward_list<int> extra{50, 60};
    lst.splice_after(lst.before_begin(), extra);
    print(lst, "splice_after");
    std::cout << "extra empty=" << extra.empty() << "\n";

    assert(std::ranges::is_sorted(merge_sorted({5,1,3},{4,2,6})));
    assert(last_element({1,2,3}).value() == 3);
    assert(!last_element({}).has_value());
    assert(average({2,4,6}).value() == 4.0);
    assert(filter_list({1,2,3,4}, [](int x){ return x%2==0; }).front() == 2);

    lst.clear();
    std::cout << "\nclear empty=" << std::boolalpha << lst.empty() << "\n";
    std::cout << "\nAll assertions passed.\n";
    return 0;
}
