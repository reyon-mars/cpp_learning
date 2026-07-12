#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <ranges>
#include <format>
#include <cassert>
#include <functional>
#include <span>
#include <array>
#include <string>
#include <map>

template <std::ranges::input_range R>
void print_range(std::string_view label, R&& r) {
    std::cout << std::format("{}: ", label);
    for (const auto& v : r)
        std::cout << std::format("{} ", v);
    std::cout << '\n';
}

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5, 3, 2, 1};

    if (auto it = std::ranges::find(vec, 3); it != vec.end())
        std::cout << std::format("Found 3 at index: {}\n",
                                 std::ranges::distance(vec.begin(), it));

    std::cout << std::format("Count of 3: {}\n", std::ranges::count(vec, 3));

    if (auto it = std::ranges::find_if(vec, [](int x) { return x % 2 == 0; });
        it != vec.end())
        std::cout << std::format("First even: {}\n", *it);

    const std::vector<int> pattern = {3, 2};
    if (auto sub = std::ranges::search(vec, pattern); !sub.empty())
        std::cout << std::format("Subsequence {{3,2}} found at index: {}\n",
                                 std::ranges::distance(vec.begin(), sub.begin()));

    std::cout << std::format("All positive:        {}\n",
                             std::ranges::all_of(vec,  [](int x) { return x > 0; }) ? "Yes" : "No");
    std::cout << std::format("Any greater than 4:  {}\n",
                             std::ranges::any_of(vec,  [](int x) { return x > 4; }) ? "Yes" : "No");
    std::cout << std::format("No negative numbers: {}\n",
                             std::ranges::none_of(vec, [](int x) { return x < 0; }) ? "Yes" : "No");

    std::cout << std::format("Even count: {}\n",
                             std::ranges::count_if(vec, [](int x) { return x % 2 == 0; }));

    if (std::ranges::find_if_not(vec, [](int x) { return x > 0; }) == vec.end())
        std::cout << "All elements are positive (find_if_not check)\n";

    std::vector<int> vec2 = vec;
    std::cout << std::format("vec and vec2 are equal: {}\n",
                             std::ranges::equal(vec, vec2) ? "Yes" : "No");

    vec2[0] = 99;
    if (auto [a, b] = std::ranges::mismatch(vec, vec2); a != vec.end())
        std::cout << std::format("Mismatch: {} vs {}\n", *a, *b);

    std::cout << std::format("Sum of elements: {}\n",
                             std::reduce(vec.begin(), vec.end(), 0));

    if (auto sub = std::ranges::find_end(vec, pattern); !sub.empty())
        std::cout << std::format("Last {{3,2}} at index: {}\n",
                                 std::ranges::distance(vec.begin(), sub.begin()));

    if (auto it = std::ranges::adjacent_find(vec); it != vec.end())
        std::cout << std::format("First adjacent duplicate: {}\n", *it);
    else
        std::cout << "No adjacent duplicates found\n";

    if (auto sub = std::ranges::search_n(vec, 2, 3); !sub.empty())
        std::cout << std::format("Two consecutive 3s at index: {}\n",
                                 std::ranges::distance(vec.begin(), sub.begin()));
    else
        std::cout << "No consecutive 3s found\n";

    std::cout << std::format("Min: {}\n", *std::ranges::min_element(vec));
    std::cout << std::format("Max: {}\n", *std::ranges::max_element(vec));

    auto [mn, mx] = std::ranges::minmax_element(vec);
    std::cout << std::format("minmax_element: min={}, max={}\n", *mn, *mx);

    std::vector<int> sorted_vec = vec;
    std::ranges::sort(sorted_vec);

    std::cout << std::format("Binary search for 4: {}\n",
                             std::ranges::binary_search(sorted_vec, 4) ? "Found" : "Not found");

    std::cout << std::format("Lower bound of 3 at index: {}\n",
                             std::ranges::distance(sorted_vec.begin(),
                                                   std::ranges::lower_bound(sorted_vec, 3)));

    std::cout << std::format("Upper bound of 3 at index: {}\n",
                             std::ranges::distance(sorted_vec.begin(),
                                                   std::ranges::upper_bound(sorted_vec, 3)));

    auto [lo, hi] = std::ranges::equal_range(sorted_vec, 2);
    std::cout << std::format("Equal range for 2: {} to {}\n",
                             std::ranges::distance(sorted_vec.begin(), lo),
                             std::ranges::distance(sorted_vec.begin(), hi));

    std::cout << std::format("sorted_vec is sorted: {}\n",
                             std::ranges::is_sorted(sorted_vec) ? "Yes" : "No");

    std::vector<int> partial(vec.size());
    std::partial_sum(vec.begin(), vec.end(), partial.begin());
    print_range("Partial sums", partial);

    std::vector<int> inc(vec.size());
    std::inclusive_scan(vec.begin(), vec.end(), inc.begin());
    assert(inc == partial);
    std::cout << "inclusive_scan matches partial_sum\n";

    std::vector<int> exc(vec.size());
    std::exclusive_scan(vec.begin(), vec.end(), exc.begin(), 0);
    print_range("Exclusive scan", exc);

    const int dot = std::inner_product(vec.begin(), vec.end(), vec.begin(), 0);
    std::cout << std::format("Inner product (dot with self): {}\n", dot);

    auto pipeline = vec
        | std::views::filter([](int x) { return x % 2 == 0; })
        | std::views::transform([](int x) { return x * x; })
        | std::views::take(3);
    print_range("Even squares (first 3)", pipeline);

    const bool part = std::ranges::is_partitioned(sorted_vec, [](int x) { return x <= 3; });
    std::cout << std::format("sorted_vec partitioned at <=3: {}\n", part ? "Yes" : "No");

    auto pp = std::ranges::partition_point(sorted_vec, [](int x) { return x <= 3; });
    std::cout << std::format("Partition point index: {}\n",
                             std::ranges::distance(sorted_vec.begin(), pp));

    std::vector<int> iota_vec(5);
    std::iota(iota_vec.begin(), iota_vec.end(), 1);
    print_range("iota [1..5]", iota_vec);
    print_range("views::iota [1..5]", std::views::iota(1, 6));

    std::cout << "\n--- New Additions ---\n";

    std::vector<int> to_rotate = {1, 2, 3, 4, 5};
    auto mid = std::ranges::next(to_rotate.begin(), 2);
    std::ranges::rotate(to_rotate, mid);
    print_range("rotate by 2", to_rotate);

    std::vector<int> to_reverse = {1, 2, 3, 4, 5};
    std::ranges::reverse(to_reverse);
    print_range("reversed", to_reverse);

    std::vector<int> with_dups = {1, 1, 2, 2, 3, 3, 4};
    auto [new_end, _] = std::ranges::unique(with_dups);
    with_dups.erase(new_end, with_dups.end());
    print_range("unique", with_dups);

    std::vector<int> a = {1, 3, 5}, b = {2, 4, 6};
    std::vector<int> merged;
    merged.reserve(a.size() + b.size());
    std::ranges::merge(a, b, std::back_inserter(merged));
    print_range("merged", merged);

    std::vector<int> set_a = {1, 2, 3, 4}, set_b = {3, 4, 5, 6};
    std::vector<int> intersection, difference, sym_diff;
    std::ranges::set_intersection(set_a, set_b,  std::back_inserter(intersection));
    std::ranges::set_difference(set_a, set_b,    std::back_inserter(difference));
    std::ranges::set_symmetric_difference(set_a, set_b, std::back_inserter(sym_diff));
    print_range("set_intersection",         intersection);
    print_range("set_difference",           difference);
    print_range("set_symmetric_difference", sym_diff);

    std::vector<int> heap_vec = {3, 1, 4, 1, 5, 9};
    std::ranges::make_heap(heap_vec);
    std::cout << std::format("is_heap after make_heap: {}\n",
                             std::ranges::is_heap(heap_vec) ? "Yes" : "No");
    std::ranges::sort_heap(heap_vec);
    print_range("sort_heap result", heap_vec);

    print_range("views::reverse",
                std::views::iota(1, 6) | std::views::reverse);

    print_range("views::drop(2)",
                std::views::iota(1, 6) | std::views::drop(2));

    print_range("stride(2) [C++23 preview via iota filter]",
                std::views::iota(0, 10) | std::views::filter([](int x) { return x % 2 == 0; }));

    std::vector<int> zip_a = {1, 2, 3};
    std::vector<int> zip_b = {10, 20, 30};
    std::cout << "zip products: ";
    for (auto [x, y] : std::views::zip(zip_a, zip_b))
        std::cout << std::format("{} ", x * y);
    std::cout << '\n';

    std::vector<int> flat = {1, 2, 3, 4, 5, 6};
    std::vector<int> chunked_sums;
    for (auto chunk : flat | std::views::chunk(2)) {
        chunked_sums.push_back(std::reduce(chunk.begin(), chunk.end(), 0));
    }
    print_range("chunk(2) sums", chunked_sums);

    assert(std::ranges::is_sorted(sorted_vec));
    assert(std::ranges::binary_search(sorted_vec, 3));
    assert(merged.size() == 6);
    assert(intersection == std::vector<int>{3, 4});
    assert(difference   == std::vector<int>{1, 2});
    assert(std::ranges::is_heap(heap_vec) == false);

    std::cout << "\nAll assertions passed.\n";
    return 0;
}
