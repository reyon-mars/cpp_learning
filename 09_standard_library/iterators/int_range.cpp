#include <iostream>
#include <numeric>
#include <algorithm>
#include <iterator>
#include <optional>
#include <ranges>

class int_range {
public:
    class Iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type        = int;
        using difference_type   = std::ptrdiff_t;
        using pointer           = const int*;
        using reference         = const int&;

        explicit Iterator(int v) noexcept : current_{v} {}

        [[nodiscard]] int operator*()  const noexcept { return current_; }
        [[nodiscard]] bool operator==(const Iterator&) const noexcept = default;

        Iterator& operator++()    noexcept { ++current_; return *this; }
        Iterator  operator++(int) noexcept { auto tmp = *this; ++current_; return tmp; }

    private:
        int current_;
    };

    explicit int_range(int first, int last) noexcept : start_{first}, end_{last} {}

    [[nodiscard]] Iterator begin() const noexcept { return Iterator{start_}; }
    [[nodiscard]] Iterator end()   const noexcept { return Iterator{end_};   }
    [[nodiscard]] bool     empty() const noexcept { return start_ >= end_;   }
    [[nodiscard]] int      size()  const noexcept { return end_ - start_;    }

private:
    int start_;
    int end_;
};

[[nodiscard]] bool contains(const int_range& r, int v) {
    return std::ranges::find(r, v) != r.end();
}

[[nodiscard]] int range_sum(const int_range& r) noexcept {
    return std::accumulate(r.begin(), r.end(), 0);
}

[[nodiscard]] std::optional<double> range_average(const int_range& r) {
    if (r.empty()) return std::nullopt;
    return static_cast<double>(range_sum(r)) / r.size();
}

[[nodiscard]] std::optional<int> range_min(const int_range& r) {
    if (r.empty()) return std::nullopt;
    return *std::ranges::min_element(r);
}

[[nodiscard]] std::optional<int> range_max(const int_range& r) {
    if (r.empty()) return std::nullopt;
    return *std::ranges::max_element(r);
}

[[nodiscard]] bool all_positive(const int_range& r) {
    return std::ranges::all_of(r, [](int v){ return v > 0; });
}

[[nodiscard]] long count_even(const int_range& r) {
    return std::ranges::count_if(r, [](int v){ return v % 2 == 0; });
}

[[nodiscard]] long count_odd(const int_range& r) {
    return std::ranges::count_if(r, [](int v){ return v % 2 != 0; });
}

[[nodiscard]] long count_multiples(const int_range& r, int divisor) {
    return std::ranges::count_if(r, [divisor](int v){ return v % divisor == 0; });
}

void print_transformed(const int_range& r, auto fn, const char* label) {
    std::cout << label << ": ";
    for (int v : r) std::cout << fn(v) << " ";
    std::cout << "\n";
}

void print_filtered(const int_range& r, auto pred, const char* label) {
    std::cout << label << ": ";
    for (int v : r) if (pred(v)) std::cout << v << " ";
    std::cout << "\n";
}

int main() {
    const int_range r{1, 6};

    std::cout << "=== Range [1,6) ===\n";
    std::cout << "Values: ";
    for (int v : r) std::cout << v << " ";
    std::cout << "\n";

    std::cout << "size="  << r.size()  << "\n"
              << "empty=" << std::boolalpha << r.empty() << "\n";

    if (auto first = r.begin(); first != r.end())
        std::cout << "first=" << *first << "\n";

    std::cout << "sum="   << range_sum(r) << "\n";
    if (auto a = range_average(r)) std::cout << "avg=" << *a << "\n";
    if (auto m = range_min(r))     std::cout << "min=" << *m << "\n";
    if (auto m = range_max(r))     std::cout << "max=" << *m << "\n";

    std::cout << "contains(3)=" << contains(r, 3) << "\n"
              << "all_positive=" << all_positive(r)  << "\n"
              << "count_even="   << count_even(r)    << "\n"
              << "count_odd="    << count_odd(r)     << "\n"
              << "multiples(2)=" << count_multiples(r, 2) << "\n";

    std::cout << "\n=== Reverse view (std::views::reverse) ===\n";
    std::cout << "Reversed: ";
    for (int v : std::views::reverse(r | std::ranges::to<std::vector>()))
        std::cout << v << " ";
    std::cout << "\n";

    std::cout << "\n=== Transformations ===\n";
    print_transformed(r, [](int v){ return v * v;     }, "squares");
    print_transformed(r, [](int v){ return v * v * v; }, "cubes");

    std::cout << "\n=== Filtered ===\n";
    print_filtered(r, [](int v){ return v % 2 == 0; }, "even values");
    print_filtered(r, [](int v){ return v % 2 != 0; }, "odd values");

    return 0;
}
