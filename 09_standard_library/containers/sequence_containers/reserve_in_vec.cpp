#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <optional>
#include <string_view>
#include <span>

void print_vec(std::span<const int> v, std::string_view label = "") {
    if (!label.empty()) std::cout << label << ": ";
    for (int x : v) std::cout << x << " ";
    std::cout << "\n";
}

[[nodiscard]] int  sum_vec(std::span<const int> v) noexcept {
    return std::accumulate(v.begin(), v.end(), 0);
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
    return std::ranges::find(v, n) != v.end();
}

[[nodiscard]] long count_even(std::span<const int> v) {
    return std::ranges::count_if(v, [](int x){ return x % 2 == 0; });
}

[[nodiscard]] long count_odd(std::span<const int> v) {
    return std::ranges::count_if(v, [](int x){ return x % 2 != 0; });
}

[[nodiscard]] bool is_sorted_vec(std::span<const int> v) {
    return std::ranges::is_sorted(v);
}

void print_info(std::span<const int> v) {
    std::cout << "size="  << v.size()  << "\n"
              << "sum="   << sum_vec(v) << "\n";
    if (auto m = min_vec(v)) std::cout << "min=" << *m << "\n";
    if (auto m = max_vec(v)) std::cout << "max=" << *m << "\n";
    if (auto a = avg_vec(v)) std::cout << "avg=" << *a << "\n";
    std::cout << "even="  << count_even(v) << " odd=" << count_odd(v) << "\n";
}

void extra_demo() {
    std::cout << "\n=== extra_demo ===\n";
    std::vector<int> v{1, 2, 3, 4, 5};
    print_vec(v, "Initial");
    std::cout << "sum="        << sum_vec(v)          << "\n"
              << std::boolalpha
              << "contains(3)=" << contains_value(v, 3) << "\n";
    if (auto m = max_vec(v)) std::cout << "max=" << *m << "\n";
    if (auto m = min_vec(v)) std::cout << "min=" << *m << "\n";
    if (auto a = avg_vec(v)) std::cout << "avg=" << *a << "\n";
    std::cout << "even=" << count_even(v) << " odd=" << count_odd(v) << "\n";

    std::ranges::reverse(v);
    print_vec(v, "Reversed");
    std::ranges::sort(v);
    print_vec(v, "Sorted");

    print_info(v);

    v.push_back(10); v.push_back(20);
    print_vec(v, "After push_back");
    v.pop_back();
    print_vec(v, "After pop_back");
    std::cout << "sorted=" << is_sorted_vec(v) << "\n";
}

int main() {
    std::vector<int> vec;
    vec.reserve(28);
    for (int i = 0; i < 28; ++i) vec.push_back(i);

    std::cout << "capacity=" << vec.capacity() << " size=" << vec.size() << "\n";

    std::ranges::reverse(vec);
    print_vec(vec, "Reversed");

    std::cout << "\n=== Info after reverse ===\n";
    print_info(vec);
    std::cout << "sorted=" << std::boolalpha << is_sorted_vec(vec) << "\n";

    std::ranges::sort(vec);
    print_vec(vec, "Sorted");
    std::cout << "front=" << vec.front() << " back=" << vec.back() << "\n";

    extra_demo();

    return 0;
}
