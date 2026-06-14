#include <forward_list>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <optional>

void print(const std::forward_list<int>& lst, const char* label = nullptr) {
    if (label) std::cout << label << ": ";
    for (int v : lst) std::cout << v << " ";
    std::cout << "\n";
}

[[nodiscard]] std::optional<int> first_greater_than(const std::forward_list<int>& lst, int n) {
    auto it = std::ranges::find_if(lst, [n](int v){ return v > n; });
    return it != lst.end() ? std::optional{*it} : std::nullopt;
}

int main() {
    std::forward_list<int> lst{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    auto prev = lst.before_begin();
    auto curr = lst.begin();
    while (curr != lst.end()) {
        if (*curr % 2)
            curr = lst.erase_after(prev);
        else {
            prev = curr;
            ++curr;
        }
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
        auto tail = lst.begin();
        while (std::next(tail) != lst.end()) ++tail;

        std::cout << "front=" << lst.front() << "\n"
                  << "last="  << *tail       << "\n"
                  << "max="   << *std::ranges::max_element(lst) << "\n"
                  << "min="   << *std::ranges::min_element(lst) << "\n"
                  << "avg="   << static_cast<double>(sum) / n   << "\n";
    } else {
        std::cout << "List is empty\n";
    }

    std::cout << std::boolalpha
              << "all_even="    << std::ranges::all_of(lst, [](int v){ return v % 2 == 0; }) << "\n"
              << "contains(6)=" << (std::ranges::find(lst, 6) != lst.end())                   << "\n"
              << "even_count="  << std::ranges::count_if(lst, [](int v){ return v % 2 == 0; }) << "\n"
              << "sorted="      << std::ranges::is_sorted(lst)                                 << "\n";

    if (auto v = first_greater_than(lst, 4))
        std::cout << "first>4=" << *v << "\n";

    lst.reverse();
    print(lst, "Reversed");

    lst.sort();
    print(lst, "Sorted");

    lst.push_front(100);
    print(lst, "push_front(100)");

    lst.pop_front();
    print(lst, "pop_front");

    lst.remove(4);
    print(lst, "remove(4)");

    lst.push_front(2); lst.push_front(2);
    lst.unique();
    print(lst, "unique");

    lst.resize(4);
    print(lst, "resize(4)");

    lst.clear();
    std::cout << "clear empty=" << lst.empty() << "\n";

    return 0;
}
