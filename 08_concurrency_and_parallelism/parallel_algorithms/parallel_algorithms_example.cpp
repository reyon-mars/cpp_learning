#include <iostream>
#include <vector>
#include <algorithm>
#include <execution>
#include <numeric>
#include <chrono>
#include <string_view>
#include <span>
#include <cassert>

using Clock = std::chrono::high_resolution_clock;

template<typename Func>
void measure_time(std::string_view label, Func&& fn) {
    const auto begin = Clock::now();
    std::forward<Func>(fn)();
    const auto us = std::chrono::duration_cast<std::chrono::microseconds>(
                        Clock::now() - begin).count();
    std::cout << label << " took " << us << " us\n";
}

void print_first(std::span<const int> v, int n = 5) {
    std::cout << "(first " << n << ") ";
    for (int i = 0; i < n && i < static_cast<int>(v.size()); ++i)
        std::cout << v[i] << " ";
    std::cout << "\n";
}

[[nodiscard]] bool all_positive(std::span<const int> v) {
    return std::all_of(std::execution::par, v.begin(), v.end(),
        [](int x) { return x > 0; });
}

[[nodiscard]] std::ptrdiff_t count_even(std::span<const int> v) {
    return std::count_if(std::execution::par, v.begin(), v.end(),
        [](int x) { return x % 2 == 0; });
}

[[nodiscard]] std::optional<int> find_value(std::span<const int> v, int target) {
    auto it = std::find(std::execution::par, v.begin(), v.end(), target);
    return it != v.end() ? std::optional{*it} : std::nullopt;
}

[[nodiscard]] std::vector<int> prefix_sum(std::span<const int> v) {
    std::vector<int> result(v.size());
    std::inclusive_scan(std::execution::par, v.begin(), v.end(), result.begin());
    return result;
}

[[nodiscard]] std::vector<int> adjacent_diff(std::span<const int> v) {
    std::vector<int> result(v.size());
    std::adjacent_difference(std::execution::par, v.begin(), v.end(), result.begin());
    return result;
}

void replace_even_with_zero(std::vector<int>& v) {
    std::replace_if(std::execution::par, v.begin(), v.end(),
        [](int x) { return x % 2 == 0; }, 0);
}

void erase_odds(std::vector<int>& v) {
    v.erase(std::remove_if(std::execution::par, v.begin(), v.end(),
        [](int x) { return x % 2 != 0; }), v.end());
}

[[nodiscard]] int dot_product(std::span<const int> a, std::span<const int> b) {
    assert(a.size() == b.size());
    return std::transform_reduce(std::execution::par,
        a.begin(), a.end(), b.begin(), 0);
}

int main() {
    std::vector<int> data(1'000);
    std::iota(data.begin(), data.end(), 1);

    std::cout << "=== reduce (seq / par / par_unseq) ===\n";
    auto sum_seq = std::reduce(std::execution::seq,      data.begin(), data.end(), 0);
    auto sum_par = std::reduce(std::execution::par,      data.begin(), data.end(), 0);
    auto sum_pv  = std::reduce(std::execution::par_unseq,data.begin(), data.end(), 0);
    std::cout << "seq=" << sum_seq << " par=" << sum_par << " par_unseq=" << sum_pv << "\n";

    std::cout << "\n=== transform (square) ===\n";
    std::vector<int> squared(data.size());
    std::transform(std::execution::par, data.begin(), data.end(), squared.begin(),
        [](int x) { return x * x; });
    print_first(squared);

    std::cout << "\n=== for_each (increment) ===\n";
    std::for_each(std::execution::par, data.begin(), data.end(), [](int& x) { ++x; });
    print_first(data);

    std::cout << "\n=== sort ===\n";
    std::vector<int> unsorted{9, 4, 7, 1, 3, 6, 8, 2, 5};
    std::sort(std::execution::par, unsorted.begin(), unsorted.end());
    for (int v : unsorted) std::cout << v << " ";
    std::cout << "\n";

    std::cout << "\n=== all_positive / count_even ===\n";
    std::cout << "all_positive=" << std::boolalpha << all_positive(data) << "\n"
              << "count_even="  << count_even(data) << "\n";

    std::cout << "\n=== minmax_element ===\n";
    auto [lo, hi] = std::minmax_element(std::execution::par, data.begin(), data.end());
    std::cout << "min=" << *lo << " max=" << *hi << "\n";

    std::cout << "\n=== copy ===\n";
    std::vector<int> copied(data.size());
    std::copy(std::execution::par, data.begin(), data.end(), copied.begin());
    print_first(copied);

    std::cout << "\n=== find_value ===\n";
    if (auto v = find_value(data, 500)) std::cout << "Found=" << *v << "\n";
    else                                std::cout << "Not found\n";

    std::cout << "\n=== prefix_sum ===\n";
    print_first(prefix_sum(data));

    std::cout << "\n=== replace_even_with_zero ===\n";
    replace_even_with_zero(data);
    print_first(data);

    std::cout << "\n=== dot_product ===\n";
    const std::vector<int> a{1, 2, 3, 4, 5};
    const std::vector<int> b{5, 4, 3, 2, 1};
    std::cout << "dot=" << dot_product(a, b) << "\n";

    std::cout << "\n=== erase_odds ===\n";
    std::vector<int> nums{1,2,3,4,5,6,7,8,9,10};
    erase_odds(nums);
    for (int n : nums) std::cout << n << " ";
    std::cout << "\n";

    std::cout << "\n=== adjacent_diff ===\n";
    for (int n : adjacent_diff(a)) std::cout << n << " ";
    std::cout << "\n";

    std::cout << "\n=== timing ===\n";
    std::vector<int> big(100'000);
    std::iota(big.begin(), big.end(), 1);
    measure_time("seq reduce", [&]{ std::reduce(std::execution::seq, big.begin(), big.end(), 0LL); });
    measure_time("par reduce", [&]{ std::reduce(std::execution::par, big.begin(), big.end(), 0LL); });

    std::cout << "\n=== generate (sequential — no data race) ===\n";
    std::vector<int> generated(10);
    int seed = 1;
    std::generate(generated.begin(), generated.end(), [&seed]{ return seed++; });
    for (int n : generated) std::cout << n << " ";
    std::cout << "\n";

    return 0;
}
