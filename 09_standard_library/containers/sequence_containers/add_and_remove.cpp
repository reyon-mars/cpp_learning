#include <list>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <string_view>
#include <optional>

void print_list(const std::list<int>& L, std::string_view label = "") {
    if (!label.empty()) std::cout << label << ": ";
    for (int v : L) std::cout << v << " ";
    std::cout << "\n";
}

void print_reverse(const std::list<int>& L) {
    for (auto it = L.rbegin(); it != L.rend(); ++it) std::cout << *it << " ";
    std::cout << "\n";
}

[[nodiscard]] std::size_t count_odds (const std::list<int>& L) {
    return static_cast<std::size_t>(std::ranges::count_if(L, [](int v){ return v % 2 != 0; }));
}

[[nodiscard]] std::size_t count_evens(const std::list<int>& L) {
    return L.size() - count_odds(L);
}

[[nodiscard]] int sum_list(const std::list<int>& L) noexcept {
    return std::accumulate(L.cbegin(), L.cend(), 0);
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
    print_list(lst, "Processed");
    std::cout << "size=" << lst.size()
              << " front=" << lst.front()
              << " back="  << lst.back() << "\n";
}

int main() {
    std::cout << "=== process_list (duplicate odds, erase evens) ===\n";
    process_list();

    std::cout << "\n=== demo list ===\n";
    std::list<int> demo{2, 4, 6, 8};
    print_list(demo, "Initial");
    std::cout << "size="      << demo.size()          << "\n"
              << "empty="     << std::boolalpha << demo.empty() << "\n"
              << "odds="      << count_odds(demo)      << "\n"
              << "evens="     << count_evens(demo)     << "\n"
              << "sum="       << sum_list(demo)        << "\n";
    if (auto m = max_list(demo)) std::cout << "max=" << *m << "\n";
    if (auto m = min_list(demo)) std::cout << "min=" << *m << "\n";
    if (auto a = average_list(demo)) std::cout << "avg=" << *a << "\n";

    std::cout << "\n=== reverse / sort ===\n";
    demo.reverse();
    print_list(demo, "Reversed");
    demo.sort();
    print_list(demo, "Sorted");

    std::cout << "\n=== remove / unique ===\n";
    demo.remove(4);
    print_list(demo, "After remove(4)");

    demo.push_back(6); demo.push_back(6);
    demo.unique();
    print_list(demo, "After unique");

    std::cout << "\n=== merge ===\n";
    std::list<int> other{1, 3, 5};
    other.sort();
    demo.merge(other);
    print_list(demo, "After merge");

    std::cout << "\n=== splice ===\n";
    std::list<int> extra{100, 200};
    demo.splice(demo.begin(), extra);
    print_list(demo, "After splice");
    std::cout << "extra empty=" << extra.empty() << "\n";

    std::cout << "\n=== remove_if (odd) ===\n";
    demo.remove_if([](int x){ return x % 2 != 0; });
    print_list(demo, "After remove_if");
    std::cout << "empty=" << demo.empty() << "\n";
    if (auto m = min_list(demo)) std::cout << "min=" << *m << "\n";
    if (auto a = average_list(demo)) std::cout << "avg=" << *a << "\n";

    std::cout << "\n=== reverse print ===\n";
    print_reverse(demo);

    std::cout << "\n=== push_front / push_back / resize / clear ===\n";
    demo.push_front(500);
    demo.push_back(900);
    print_list(demo, "After push");
    demo.resize(5);
    print_list(demo, "After resize(5)");
    demo.clear();
    std::cout << "After clear size=" << demo.size() << "\n";

    return 0;
}
