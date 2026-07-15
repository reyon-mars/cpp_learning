#include <iostream>
#include <numeric>
#include <algorithm>
#include <iterator>
#include <optional>
#include <ranges>
#include <vector>
#include <compare>

class int_range {
public:
    class Iterator {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type        = int;
        using difference_type   = std::ptrdiff_t;
        using pointer           = const int*;
        using reference         = const int&;

        Iterator() noexcept = default;
        explicit Iterator(int v) noexcept : current_{v} {}

        [[nodiscard]] int operator*()  const noexcept { return current_; }
        [[nodiscard]] int operator[](difference_type n) const noexcept { return current_ + static_cast<int>(n); }
        [[nodiscard]] auto operator<=>(const Iterator&) const noexcept = default;

        Iterator& operator++()    noexcept { ++current_; return *this; }
        Iterator  operator++(int) noexcept { auto tmp = *this; ++current_; return tmp; }
        Iterator& operator--()    noexcept { --current_; return *this; }
        Iterator  operator--(int) noexcept { auto tmp = *this; --current_; return tmp; }

        Iterator& operator+=(difference_type n) noexcept { current_ += static_cast<int>(n); return *this; }
        Iterator& operator-=(difference_type n) noexcept { current_ -= static_cast<int>(n); return *this; }

        [[nodiscard]] friend Iterator operator+(Iterator it, difference_type n) noexcept { it += n; return it; }
        [[nodiscard]] friend Iterator operator+(difference_type n, Iterator it) noexcept { it += n; return it; }
        [[nodiscard]] friend Iterator operator-(Iterator it, difference_type n) noexcept { it -= n; return it; }
        [[nodiscard]] friend difference_type operator-(const Iterator& lhs, const Iterator& rhs) noexcept {
            return static_cast<difference_type>(lhs.current_) - static_cast<difference_type>(rhs.current_);
        }

    private:
        int current_{};
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

static_assert(std::random_access_iterator<int_range::Iterator>);
static_assert(std::ranges::random_access_range<int_range>);
static_assert(std::ranges::sized_range<int_range>);

[[nodiscard]] bool contains(const int_range& r, int v) {
    return std::ranges::find(r, v) != r.end();
}

[[nodiscard]] bool contains_sorted(const int_range& r, int v) {
    return std::ranges::binary_search(r, v);
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

[[nodiscard]] bool any_multiple_of(const int_range& r, int divisor) {
    return std::ranges::any_of(r, [divisor](int v){ return v % divisor == 0; });
}

[[nodiscard]] bool none_negative(const int_range& r) {
    return std::ranges::none_of(r, [](int v){ return v < 0; });
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

[[nodiscard]] std::optional<int> element_at(const int_range& r, std::ptrdiff_t index) {
    if (index < 0 || index >= r.size()) return std::nullopt;
    return r.begin()[index];
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

    std::cout << "distance(O(1))=" << std::ranges::distance(r) << "\n";

    std::cout << "sum="   << range_sum(r) << "\n";
    if (auto a = range_average(r)) std::cout << "avg=" << *a << "\n";
    if (auto m = range_min(r))     std::cout << "min=" << *m << "\n";
    if (auto m = range_max(r))     std::cout << "max=" << *m << "\n";

    std::cout << "contains(3)="        << contains(r, 3)        << "\n"
              << "contains_sorted(3)=" << contains_sorted(r, 3) << "\n"
              << "all_positive="       << all_positive(r)       << "\n"
              << "none_negative="      << none_negative(r)      << "\n"
              << "any_multiple_of(3)=" << any_multiple_of(r, 3) << "\n"
              << "count_even="         << count_even(r)         << "\n"
              << "count_odd="          << count_odd(r)          << "\n"
              << "multiples(2)="       << count_multiples(r, 2) << "\n";

    if (auto v = element_at(r, 3)) std::cout << "element_at(3)=" << *v << "\n";

    std::cout << "\n=== Random access ===\n";
    auto it = r.begin();
    std::cout << "it[2]=" << it[2] << " (it+4)=" << *(it + 4) << " (end-begin)=" << (r.end() - r.begin()) << "\n";

    std::cout << "\n=== Reverse view (std::views::reverse) ===\n";
    std::cout << "Reversed: ";
    for (int v : r | std::views::reverse) std::cout << v << " ";
    std::cout << "\n";

    std::cout << "\n=== View pipeline (filter | transform | take) ===\n";
    auto pipeline = r
        | std::views::filter([](int v) { return v % 2 != 0; })
        | std::views::transform([](int v) { return v * 10; })
        | std::views::take(2);
    std::cout << "odd*10 (first 2): ";
    for (int v : pipeline) std::cout << v << " ";
    std::cout << "\n";

    std::cout << "\n=== drop / take_while / drop_while ===\n";
    std::cout << "drop(2): ";
    for (int v : r | std::views::drop(2)) std::cout << v << " ";
    std::cout << "\n";
    std::cout << "take_while(<4): ";
    for (int v : r | std::views::take_while([](int v) { return v < 4; })) std::cout << v << " ";
    std::cout << "\n";
    std::cout << "drop_while(<4): ";
    for (int v : r | std::views::drop_while([](int v) { return v < 4; })) std::cout << v << " ";
    std::cout << "\n";

    std::cout << "\n=== Materialized copy (vector<int>) ===\n";
    std::vector<int> materialized(r.begin(), r.end());
    for (int v : materialized) std::cout << v << " ";
    std::cout << "\n";

    std::cout << "\n=== Equality against std::views::iota ===\n";
    std::cout << "equal to iota(1,6): " << std::ranges::equal(r, std::views::iota(1, 6)) << "\n";

    std::cout << "\n=== Transformations ===\n";
    print_transformed(r, [](int v){ return v * v;     }, "squares");
    print_transformed(r, [](int v){ return v * v * v; }, "cubes");

    std::cout << "\n=== Filtered ===\n";
    print_filtered(r, [](int v){ return v % 2 == 0; }, "even values");
    print_filtered(r, [](int v){ return v % 2 != 0; }, "odd values");

    return 0;
}
