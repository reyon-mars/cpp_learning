#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <ranges>
#include <span>

template <std::ranges::input_range R>
void print_range(std::string_view label, const R& range) {
    std::cout << label << ": ";
    for (const auto& v : range) std::cout << v << ' ';
    std::cout << '\n';
}

template <std::ranges::random_access_range R>
std::ptrdiff_t iter_index(const R& range, std::ranges::iterator_t<const R> it) {
    return std::ranges::distance(std::ranges::begin(range), it);
}

int main() {
    std::vector data {5, 2, 8, 1, 9, 3};

    std::ranges::sort(data);
    print_range("Sorted", data);

    data = {5, 2, 8, 1, 9, 3};
    std::ranges::stable_sort(data);
    print_range("Stable sorted", data);

    data = {5, 2, 8, 1, 9, 3};
    std::ranges::nth_element(data, data.begin() + 2);
    std::cout << "Element at position 2: " << data[2] << '\n';

    data = {5, 2, 8, 1, 9, 3};
    auto pivot = std::ranges::partition(data, [](int x) { return x < 5; });
    std::cout << "Partition index: " << iter_index(data, pivot.begin()) << '\n';
    print_range("After partition", data);

    std::cout << "Is data sorted? "
              << (std::ranges::is_sorted(data) ? "Yes" : "No") << '\n';

    std::ranges::reverse(data);
    print_range("Reversed data", data);

    auto point = std::ranges::partition_point(data, [](int x) { return x < 5; });
    std::cout << "Partition point index: " << iter_index(data, point) << '\n';

    std::ranges::sort(data);

    std::cout << "Binary search for 5: "
              << (std::ranges::binary_search(data, 5) ? "Found" : "Not found") << '\n';

    auto lb = std::ranges::lower_bound(data, 5);
    auto ub = std::ranges::upper_bound(data, 5);
    std::cout << "Lower bound index of 5: " << iter_index(data, lb) << '\n';
    std::cout << "Upper bound index of 5: " << iter_index(data, ub) << '\n';

    auto [first, last] = std::ranges::equal_range(data, 5);
    std::cout << "Equal range for 5: ["
              << iter_index(data, first) << ", "
              << iter_index(data, last) << ")\n";

    data = {5, 2, 8, 1, 9, 3};
    std::ranges::partial_sort(data, data.begin() + 3);
    print_range("Partial sort (first 3 smallest)", data);

    data = {1, 2, 3, 6, 7, 8};
    std::cout << "Is partitioned (<5)? "
              << (std::ranges::is_partitioned(data, [](int x) { return x < 5; }) ? "Yes" : "No")
              << '\n';

    data = {5, 2, 8, 1, 9, 3};
    std::ranges::sort(data, std::greater<>{});
    print_range("Descending sort", data);

    data = {1, 2, 3, 4, 5, 6};
    std::ranges::stable_partition(data, [](int x) { return x % 2 == 0; });
    print_range("Stable partition (evens first)", data);

    std::vector merged {1, 3, 5, 2, 4, 6};
    std::inplace_merge(merged.begin(), merged.begin() + 3, merged.end());
    print_range("Inplace merged", merged);

    std::vector perm {1, 2, 3};
    std::ranges::next_permutation(perm);
    print_range("Next permutation", perm);

    std::ranges::prev_permutation(perm);
    print_range("Previous permutation", perm);

    std::vector heap_data {4, 1, 7, 3, 8, 5};
    std::ranges::make_heap(heap_data);
    std::cout << "Heap front element: " << heap_data.front() << '\n';

    std::ranges::pop_heap(heap_data);
    print_range("After pop_heap", heap_data);

    std::ranges::push_heap(heap_data);
    std::ranges::sort_heap(heap_data);
    print_range("Heap sort result", heap_data);

    data = {5, 2, 8, 1, 9, 3};
    auto [min_it, max_it] = std::ranges::minmax_element(data);
    std::cout << "Min: " << *min_it << "  Max: " << *max_it << '\n';

    std::vector<int> sorted_copy(data.size());
    std::ranges::partial_sort_copy(data, sorted_copy);
    print_range("Partial sort copy", sorted_copy);

    return 0;
}
