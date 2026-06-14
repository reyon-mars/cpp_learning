#include <iostream>
#include <list>
#include <numeric>
#include <algorithm>
#include <optional>
#include <string_view>

void print_list(const std::list<int>& lst, std::string_view label = "") {
    if (!label.empty()) std::cout << label << ": ";
    for (int x : lst) std::cout << x << " ";
    std::cout << "\n";
}

[[nodiscard]] int  sum_list(const std::list<int>& lst) noexcept {
    return std::accumulate(lst.cbegin(), lst.cend(), 0);
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
    return std::ranges::all_of(lst, [](int v){ return v > 0; });
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
    std::cout << "size="   << v.size()  << "\n"
              << "empty="  << std::boolalpha << v.empty() << "\n"
              << "sum="    << sum_list(v) << "\n";

    if (auto m = max_list(v))     std::cout << "max="  << *m << "\n";
    if (auto m = min_list(v))     std::cout << "min="  << *m << "\n";
    if (auto a = average_list(v)) std::cout << "avg="  << *a << "\n";

    std::cout << "contains(42)=" << contains_value(v, 42) << "\n"
              << "count(42)="    << count_value(v, 42)    << "\n";

    std::cout << "\n=== reverse / sort / remove / unique ===\n";
    v.reverse();
    print_list(v, "Reversed");
    v.sort();
    print_list(v, "Sorted");
    v.remove(42);
    print_list(v, "After remove(42)");
    v.push_back(1); v.push_back(1);
    v.unique();
    print_list(v, "After unique");

    std::cout << "\n=== push/pop / front / back ===\n";
    v.push_front(100);
    v.push_back(200);
    print_list(v, "After push_front/back");
    std::cout << "front=" << v.front() << " back=" << v.back() << "\n";
    v.pop_front();
    v.pop_back();
    print_list(v, "After pop_front/back");
    std::cout << "all_positive=" << all_positive(v) << "\n";

    std::cout << "\n=== resize / clear ===\n";
    v.resize(6, 9);
    print_list(v, "After resize(6,9)");

    std::list<int> temp = v;
    temp.clear();
    std::cout << "temp after clear: size=" << temp.size() << " empty=" << temp.empty() << "\n";

    return 0;
}
