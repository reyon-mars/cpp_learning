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

template <std::ranges::input_range R>
void print_range(std::string_view label, R&& r) {
    std::cout << std::format("{}: ", label);
    for (const auto& v : r)
        std::cout << std::format("{} ", v);
    std::cout << '\n';
}

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5, 3, 2, 1};

    // find
    if (auto it = std::ranges::find(vec, 3); it != vec.end())
        std::cout << std::format("Found 3 at index: {}\n",
                                 std::ranges::distance(vec.begin(), it));

    // count
    std::cout << std::format("Count of 3: {}\n", std::ranges::count(vec, 3));

    // find_if
    if (auto it = std::ranges::find_if(vec, [](int x) { return x % 2 == 0; });
        it != vec.end())
        std::cout << std::format("First even: {}\n", *it);

    // search (subsequence)
    const std::vector<int> pattern = {3, 2};
    if (auto sub = std::ranges::search(vec, pattern); !sub.empty())
        std::cout << std::format("Subsequence {{3,2}} found at index: {}\n",
                                 std::ranges::distance(vec.begin(), sub.begin()));

    // all_of / any_of / none_of
    std::cout << std::format("All positive:        {}\n",
                             std::ranges::all_of(vec,  [](int x) { return x > 0; }) ? "Yes" : "No");
    std::cout << std::format("Any greater than 4:  {}\n",
                             std::ranges::any_of(vec,  [](int x) { return x > 4; }) ? "Yes" : "No");
    std::cout << std::format("No negative numbers: {}\n",
                             std::ranges::none_of(vec, [](int x) { return x < 0; }) ? "Yes" : "No");

    // count_if
    std::cout << std::format("Even count: {}\n",
                             std::ranges::count_if(vec, [](int x) { return x % 2 == 0; }));

    // find_if_not
    if (std::ranges::find_if_not(vec, [](int x) { return x > 0; }) == vec.end())
        std::cout << "All elements are positive (find_if_not check)\n";

    // equal / mismatch
    std::vector<int> vec2 = vec;
    std::cout << std::format("vec and vec2 are equal: {}\n",
                             std::ranges::equal(vec, vec2) ? "Yes" : "No");

    vec2[0] = 99;
    if (auto [a, b] = std::ranges::mismatch(vec, vec2); a != vec.end())
        std::cout << std::format("Mismatch: {} vs {}\n", *a, *b);

    // reduce (prefer over accumulate for parallelism-readiness)
    std::cout << std::format("Sum of elements: {}\n",
                             std::reduce(vec.begin(), vec.end(), 0));

    // find_end
    if (auto sub = std::ranges::find_end(vec, pattern); !sub.empty())
        std::cout << std::format("Last {{3,2}} at index: {}\n",
                                 std::ranges::distance(vec.begin(), sub.begin()));

    // adjacent_find
    if (auto it = std::ranges::adjacent_find(vec); it != vec.end())
        std::cout << std::format("First adjacent duplicate: {}\n", *it);
    else
        std::cout << "No adjacent duplicates found\n";

    // search_n
    if (auto sub = std::ranges::search_n(vec, 2, 3); !sub.empty())
        std::cout << std::format("Two consecutive 3s at index: {}\n",
                                 std::ranges::distance(vec.begin(), sub.begin()));
    else
        std::cout << "No consecutive 3s found\n";

    // min / max element
    std::cout << std::format("Min: {}\n", *std::ranges::min_element(vec));
    std::cout << std::format("Max: {}\n", *std::ranges::max_element(vec));

    // minmax_element (single-pass)
    auto [mn, mx] = std::ranges::minmax_element(vec);
    std::cout << std::format("minmax_element: min={}, max={}\n", *mn, *mx);

    // binary_search / lower_bound / upper_bound / equal_range
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

    // partial_sum
    std::vector<int> partial(vec.size());
    std::partial_sum(vec.begin(), vec.end(), partial.begin());
    print_range("Partial sums", partial);

    // inclusive_scan (C++17 parallel-ready replacement for partial_sum)
    std::vector<int> inc(vec.size());
    std::inclusive_scan(vec.begin(), vec.end(), inc.begin());
    assert(inc == partial);
    std::cout << "inclusive_scan matches partial_sum\n";

    // exclusive_scan
    std::vector<int> exc(vec.size());
    std::exclusive_scan(vec.begin(), vec.end(), exc.begin(), 0);
    print_range("Exclusive scan", exc);

    // inner_product
    const int dot = std::inner_product(vec.begin(), vec.end(), vec.begin(), 0);
    std::cout << std::format("Inner product (dot with self): {}\n", dot);

    // ranges views pipeline: filter evens, transform to squares, take first 3
    auto pipeline = vec
        | std::views::filter([](int x) { return x % 2 == 0; })
        | std::views::transform([](int x) { return x * x; })
        | std::views::take(3);
    print_range("Even squares (first 3)", pipeline);

    // is_partitioned / partition_point
    const bool part = std::ranges::is_partitioned(sorted_vec, [](int x) { return x <= 3; });
    std::cout << std::format("sorted_vec partitioned at <=3: {}\n", part ? "Yes" : "No");

    auto pp = std::ranges::partition_point(sorted_vec, [](int x) { return x <= 3; });
    std::cout << std::format("Partition point index: {}\n",
                             std::ranges::distance(sorted_vec.begin(), pp));

    // iota + ranges
    std::vector<int> iota_vec(5);
    std::iota(iota_vec.begin(), iota_vec.end(), 1);
    print_range("iota [1..5]", iota_vec);

    // views::iota (lazy, no allocation)
    print_range("views::iota [1..5]", std::views::iota(1, 6));

    return 0;
}
