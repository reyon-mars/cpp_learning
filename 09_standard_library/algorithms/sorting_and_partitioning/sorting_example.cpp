#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <ranges>
#include <span>
#include <format>
#include <cassert>
#include <array>
#include <numeric>
#include <concepts>

template <std::ranges::input_range R>
void print_range(std::string_view label, const R& range) {
    std::cout << std::format("{}: ", label);
    for (const auto& v : range) std::cout << std::format("{} ", v);
    std::cout << '\n';
}

template <std::ranges::random_access_range R>
[[nodiscard]] std::ptrdiff_t iter_index(const R& range,
                                         std::ranges::iterator_t<const R> it) {
    return std::ranges::distance(std::ranges::begin(range), it);
}

template <std::ranges::random_access_range R>
[[nodiscard]] std::vector<std::size_t> sort_indices(const R& range) {
    std::vector<std::size_t> idx(std::ranges::size(range));
    std::iota(idx.begin(), idx.end(), 0u);
    std::ranges::sort(idx, [&](std::size_t a, std::size_t b) {
        return range[a] < range[b];
    });
    return idx;
}

template <std::ranges::random_access_range R, std::strict_weak_order<
    std::ranges::range_value_t<R>, std::ranges::range_value_t<R>> Comp = std::ranges::less>
[[nodiscard]] bool is_sorted_by(const R& range, Comp cmp = {}) {
    return std::ranges::is_sorted(range, cmp);
}

int main() {
    std::vector data{5, 2, 8, 1, 9, 3};

    std::ranges::sort(data);
    print_range("Sorted", data);

    data = {5, 2, 8, 1, 9, 3};
    std::ranges::stable_sort(data);
    print_range("Stable sorted", data);

    data = {5, 2, 8, 1, 9, 3};
    std::ranges::nth_element(data, data.begin() + 2);
    std::cout << std::format("Element at position 2: {}\n", data[2]);

    data = {5, 2, 8, 1, 9, 3};
    auto pivot = std::ranges::partition(data, [](int x) { return x < 5; });
    std::cout << std::format("Partition index: {}\n", iter_index(data, pivot.begin()));
    print_range("After partition", data);

    std::cout << std::format("Is data sorted? {}\n",
                             std::ranges::is_sorted(data) ? "Yes" : "No");

    std::ranges::reverse(data);
    print_range("Reversed data", data);

    auto point = std::ranges::partition_point(data, [](int x) { return x < 5; });
    std::cout << std::format("Partition point index: {}\n", iter_index(data, point));

    std::ranges::sort(data);
    std::cout << std::format("Binary search for 5: {}\n",
                             std::ranges::binary_search(data, 5) ? "Found" : "Not found");

    auto lb = std::ranges::lower_bound(data, 5);
    auto ub = std::ranges::upper_bound(data, 5);
    std::cout << std::format("Lower bound index of 5: {}\n", iter_index(data, lb));
    std::cout << std::format("Upper bound index of 5: {}\n", iter_index(data, ub));

    auto [first, last] = std::ranges::equal_range(data, 5);
    std::cout << std::format("Equal range for 5: [{}, {})\n",
                             iter_index(data, first), iter_index(data, last));

    data = {5, 2, 8, 1, 9, 3};
    std::ranges::partial_sort(data, data.begin() + 3);
    print_range("Partial sort (first 3 smallest)", data);

    data = {1, 2, 3, 6, 7, 8};
    std::cout << std::format("Is partitioned (<5)? {}\n",
                             std::ranges::is_partitioned(data, [](int x) { return x < 5; })
                             ? "Yes" : "No");

    data = {5, 2, 8, 1, 9, 3};
    std::ranges::sort(data, std::greater<>{});
    print_range("Descending sort", data);

    data = {1, 2, 3, 4, 5, 6};
    std::ranges::stable_partition(data, [](int x) { return x % 2 == 0; });
    print_range("Stable partition (evens first)", data);

    std::vector merged{1, 3, 5, 2, 4, 6};
    std::inplace_merge(merged.begin(), merged.begin() + 3, merged.end());
    print_range("Inplace merged", merged);

    std::vector perm{1, 2, 3};
    std::ranges::next_permutation(perm);
    print_range("Next permutation", perm);
    std::ranges::prev_permutation(perm);
    print_range("Previous permutation", perm);

    std::vector heap_data{4, 1, 7, 3, 8, 5};
    std::ranges::make_heap(heap_data);
    std::cout << std::format("Heap front element: {}\n", heap_data.front());
    std::ranges::pop_heap(heap_data);
    print_range("After pop_heap", heap_data);
    std::ranges::push_heap(heap_data);
    std::ranges::sort_heap(heap_data);
    print_range("Heap sort result", heap_data);

    data = {5, 2, 8, 1, 9, 3};
    auto [min_it, max_it] = std::ranges::minmax_element(data);
    std::cout << std::format("Min: {}  Max: {}\n", *min_it, *max_it);

    std::vector<int> sorted_copy(data.size());
    std::ranges::partial_sort_copy(data, sorted_copy);
    print_range("Partial sort copy", sorted_copy);

    std::cout << "\n--- New Additions ---\n";

    std::vector<int> src{5, 2, 8, 1, 9, 3};
    const auto indices = sort_indices(src);
    std::cout << "Sort indices: ";
    for (std::size_t i : indices) std::cout << std::format("{} ", i);
    std::cout << '\n';
    print_range("Values in sorted order via indices",
                indices | std::views::transform([&](std::size_t i) { return src[i]; }));

    std::vector<int> with_dupes{3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
    std::ranges::sort(with_dupes);
    auto [uniq_end, _] = std::ranges::unique(with_dupes);
    with_dupes.erase(uniq_end, with_dupes.end());
    print_range("Sorted unique", with_dupes);

    std::vector<int> to_rotate{1, 2, 3, 4, 5};
    for (int k : {1, 2, 3}) {
        auto rotated = to_rotate;
        std::ranges::rotate(rotated, rotated.begin() + k);
        print_range(std::format("rotate({})", k), rotated);
    }

    std::vector<int> all_perms{1, 2, 3};
    std::cout << "All permutations of {1,2,3}:\n";
    do {
        for (int v : all_perms) std::cout << std::format("{} ", v);
        std::cout << '\n';
    } while (std::ranges::next_permutation(all_perms).found);

    std::vector<int> sample{3, 1, 4, 1, 5, 9, 2, 6};
    std::ranges::sort(sample);
    const int median = sample[sample.size() / 2];
    std::cout << std::format("Median (upper): {}\n", median);

    const auto kth = [](std::vector<int> v, std::size_t k) {
        std::ranges::nth_element(v, v.begin() + static_cast<std::ptrdiff_t>(k));
        return v[k];
    };
    std::cout << std::format("3rd smallest of {{5,2,8,1,9,3}}: {}\n",
                             kth({5, 2, 8, 1, 9, 3}, 2));

    constexpr std::array by_abs{-3, 1, -5, 2, -4};
    std::vector<int> abs_sorted(by_abs.begin(), by_abs.end());
    std::ranges::sort(abs_sorted, [](int a, int b) {
        return std::abs(a) < std::abs(b);
    });
    print_range("Sorted by |value|", abs_sorted);

    assert(std::ranges::is_sorted(sorted_copy));
    assert(std::ranges::binary_search(sorted_copy, 5));
    assert(*std::ranges::min_element(src) == 1);
    assert(*std::ranges::max_element(src) == 9);
    assert(kth({5, 2, 8, 1, 9, 3}, 0) == 1);
    assert(std::ranges::is_sorted(with_dupes));

    std::cout << "\nAll assertions passed.\n";
    return 0;
}
