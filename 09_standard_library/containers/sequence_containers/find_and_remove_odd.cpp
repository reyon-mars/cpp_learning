#include <forward_list>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <optional>
#include <string_view>
#include <cstddef>

void print_list(const std::forward_list<int>& lst, std::string_view label = "") {
    if (!label.empty()) std::cout << label << ": ";
    for (int v : lst) std::cout << "[" << v << "]->";
    std::cout << "null\n";
}

void print_first_n(const std::forward_list<int>& lst, int n) {
    int i = 0;
    for (int v : lst) {
        if (i++ >= n) break;
        std::cout << v << " ";
    }
    std::cout << "\n";
}

[[nodiscard]] std::size_t count_elements(const std::forward_list<int>& lst) noexcept {
    return static_cast<std::size_t>(std::ranges::distance(lst));
}

[[nodiscard]] int  sum_elements     (const std::forward_list<int>& lst) noexcept {
    return std::accumulate(lst.cbegin(), lst.cend(), 0);
}

[[nodiscard]] int  multiply_elements(const std::forward_list<int>& lst) noexcept {
    return lst.empty() ? 0
                       : std::accumulate(lst.cbegin(), lst.cend(), 1, std::multiplies<>{});
}

[[nodiscard]] std::optional<double> average_elements(const std::forward_list<int>& lst) {
    auto n = count_elements(lst);
    if (n == 0) return std::nullopt;
    return static_cast<double>(sum_elements(lst)) / static_cast<double>(n);
}

[[nodiscard]] std::optional<int> max_element_safe(const std::forward_list<int>& lst) {
    if (lst.empty()) return std::nullopt;
    return *std::ranges::max_element(lst);
}

[[nodiscard]] std::optional<int> min_element_safe(const std::forward_list<int>& lst) {
    if (lst.empty()) return std::nullopt;
    return *std::ranges::min_element(lst);
}

[[nodiscard]] std::optional<int> last_element(const std::forward_list<int>& lst) {
    if (lst.empty()) return std::nullopt;
    auto it = lst.begin();
    while (std::next(it) != lst.end()) ++it;
    return *it;
}

[[nodiscard]] bool contains     (const std::forward_list<int>& lst, int v) {
    return std::ranges::find(lst, v) != lst.end();
}

[[nodiscard]] bool all_even     (const std::forward_list<int>& lst) {
    return std::ranges::all_of(lst, [](int v){ return v % 2 == 0; });
}

[[nodiscard]] long count_odds   (const std::forward_list<int>& lst) {
    return std::ranges::count_if(lst, [](int v){ return v % 2 != 0; });
}

[[nodiscard]] long count_evens  (const std::forward_list<int>& lst) {
    return std::ranges::count_if(lst, [](int v){ return v % 2 == 0; });
}

[[nodiscard]] bool is_sorted_list(const std::forward_list<int>& lst) {
    return std::ranges::is_sorted(lst);
}

void find_and_remove_odd(std::forward_list<int>& lst) {
    auto prev = lst.before_begin();
    auto curr = lst.begin();
    while (curr != lst.end()) {
        if (*curr % 2 != 0)
            curr = lst.erase_after(prev);
        else {
            prev = curr;
            ++curr;
        }
    }
}

int main() {
    std::cout << "=== fl1: mixed (remove odds) ===\n";
    std::forward_list<int> fl1{1, 2, 3, 4, 5, 6, 7, 8};
    print_list(fl1, "Original");
    find_and_remove_odd(fl1);
    print_list(fl1, "After remove_odd");
    std::cout << "count=" << count_elements(fl1) << "\n"
              << "sum="   << sum_elements(fl1)   << "\n";
    if (auto a = average_elements(fl1))   std::cout << "avg=" << *a << "\n";
    if (auto m = max_element_safe(fl1))   std::cout << "max=" << *m << "\n";
    if (auto m = min_element_safe(fl1))   std::cout << "min=" << *m << "\n";
    if (auto l = last_element(fl1))       std::cout << "last=" << *l << "\n";
    std::cout << "front=" << fl1.front() << "\n"
              << std::boolalpha
              << "contains(6)=" << contains(fl1, 6) << "\n"
              << "all_even="    << all_even(fl1)     << "\n";

    std::cout << "\n=== fl2: all odds → empty ===\n";
    std::forward_list<int> fl2{1, 3, 5, 7, 9};
    print_list(fl2, "Original");
    find_and_remove_odd(fl2);
    print_list(fl2, "After remove_odd");
    std::cout << "count=" << count_elements(fl2) << " empty=" << fl2.empty() << "\n";

    std::cout << "\n=== fl3: all evens (no change) ===\n";
    std::forward_list<int> fl3{2, 4, 6, 8, 10};
    print_list(fl3, "Original");
    find_and_remove_odd(fl3);
    print_list(fl3, "After remove_odd");
    std::cout << "count=" << count_elements(fl3) << " sum=" << sum_elements(fl3) << "\n";
    if (auto a = average_elements(fl3)) std::cout << "avg=" << *a << "\n";
    std::cout << "odds=" << count_odds(fl3) << "\n";
    fl3.clear();
    std::cout << "Cleared fl3 empty=" << fl3.empty() << "\n";

    std::cout << "\n=== fl4: empty list ===\n";
    std::forward_list<int> fl4;
    print_list(fl4, "Original");
    find_and_remove_odd(fl4);
    print_list(fl4, "After remove_odd");
    std::cout << "empty=" << fl4.empty() << "\n";

    std::cout << "\n=== fl1 manipulations ===\n";
    fl1.push_front(100);
    print_list(fl1, "push_front(100)");
    fl1.pop_front();
    print_list(fl1, "pop_front");
    fl1.reverse();
    print_list(fl1, "reverse");
    fl1.sort();
    print_list(fl1, "sort");
    fl1.remove(4);
    print_list(fl1, "remove(4)");

    fl1.push_front(2); fl1.push_front(2);
    fl1.unique();
    print_list(fl1, "unique");

    std::cout << "evens="    << count_evens(fl1)     << "\n"
              << "product="  << multiply_elements(fl1) << "\n"
              << "sorted="   << is_sorted_list(fl1)   << "\n";

    std::cout << "First 3: ";
    print_first_n(fl1, 3);

    fl1.emplace_front(500);
    print_list(fl1, "emplace_front(500)");
    fl1.resize(4);
    print_list(fl1, "resize(4)");

    std::forward_list<int> extra{50, 60};
    fl1.splice_after(fl1.before_begin(), extra);
    print_list(fl1, "splice_after");
    std::cout << "extra empty=" << extra.empty() << "\n";

    fl2.clear();
    std::cout << "fl2 after clear empty=" << fl2.empty() << "\n";

    return 0;
}
