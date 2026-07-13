#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <functional>
#include <span>
#include <ranges>
#include <cmath>
#include <format>
#include <cassert>
#include <array>

template <std::ranges::input_range R>
void print_range(std::string_view label, const R& range) {
    std::cout << std::format("{}: ", label);
    for (const auto& v : range) std::cout << std::format("{} ", v);
    std::cout << '\n';
}

template <std::ranges::input_range R>
[[nodiscard]] auto range_sum(const R& r) {
    using T = std::ranges::range_value_t<R>;
    return std::reduce(std::ranges::begin(r), std::ranges::end(r), T{});
}

template <std::ranges::input_range R>
[[nodiscard]] double mean(const R& r) {
    const auto n = static_cast<double>(std::ranges::distance(r));
    if (n == 0.0) return 0.0;
    return static_cast<double>(range_sum(r)) / n;
}

template <std::ranges::input_range R>
[[nodiscard]] double variance(const R& r) {
    const double m = mean(r);
    const auto   n = static_cast<double>(std::ranges::distance(r));
    if (n == 0.0) return 0.0;
    return std::transform_reduce(
               std::ranges::begin(r), std::ranges::end(r),
               0.0, std::plus<>{},
               [m](auto x) { const double d = static_cast<double>(x) - m; return d * d; })
           / n;
}

template <std::ranges::input_range R>
[[nodiscard]] double stddev(const R& r) { return std::sqrt(variance(r)); }

template <std::ranges::input_range R>
[[nodiscard]] double l2_norm(const R& r) {
    return std::sqrt(static_cast<double>(
        std::transform_reduce(std::ranges::begin(r), std::ranges::end(r),
                              0, std::plus<>{},
                              [](auto x) { return x * x; })));
}

int main() {
    constexpr std::array vec  {1, 2, 3, 4, 5};
    constexpr std::array vec2 {2, 3, 4, 5, 6};

    std::cout << std::format("Sum:     {}\n",
                             std::reduce(vec.begin(), vec.end(), 0));

    std::cout << std::format("Product: {}\n",
                             std::reduce(vec.begin(), vec.end(), 1, std::multiplies<>{}));

    std::cout << std::format("Dot product: {}\n",
                             std::inner_product(vec.begin(), vec.end(), vec2.begin(), 0));

    std::vector<int> partial(vec.size());
    std::partial_sum(vec.begin(), vec.end(), partial.begin());
    print_range("Partial sums", partial);

    std::vector<int> diff(vec.size());
    std::adjacent_difference(vec.begin(), vec.end(), diff.begin());
    print_range("Adjacent differences", diff);

    std::cout << std::format("Subtraction result: {}\n",
                             std::accumulate(vec.begin(), vec.end(), 0, std::minus<>{}));

    std::cout << std::format("Custom inner product: {}\n",
                             std::inner_product(
                                 vec.begin(), vec.end(), vec2.begin(), 0,
                                 std::plus<>{},
                                 [](int a, int b) { return a * b + 1; }));

    std::vector<int> partial_product(vec.size());
    std::partial_sum(vec.begin(), vec.end(), partial_product.begin(), std::multiplies<>{});
    print_range("Partial products", partial_product);

    std::vector<int> diff_product(vec.size());
    std::adjacent_difference(vec.begin(), vec.end(), diff_product.begin(), std::multiplies<>{});
    print_range("Adjacent products", diff_product);

    std::vector<int> prefix_max(vec.size());
    std::partial_sum(vec.begin(), vec.end(), prefix_max.begin(),
                     [](int a, int b) { return std::max(a, b); });
    print_range("Prefix max", prefix_max);

    std::cout << std::format("Reduce sum: {}\n",
                             std::reduce(vec.begin(), vec.end(), 0));

    std::vector<int> inclusive(vec.size());
    std::inclusive_scan(vec.begin(), vec.end(), inclusive.begin());
    print_range("Inclusive scan", inclusive);

    std::vector<int> exclusive(vec.size());
    std::exclusive_scan(vec.begin(), vec.end(), exclusive.begin(), 0);
    print_range("Exclusive scan", exclusive);

    std::vector<double> running_avg(vec.size());
    std::inclusive_scan(vec.begin(), vec.end(), running_avg.begin(), std::plus<double>{});
    for (std::size_t i = 0; i < running_avg.size(); ++i)
        running_avg[i] /= static_cast<double>(i + 1);
    print_range("Running average", running_avg);

    std::cout << std::format("Transform reduce result: {}\n",
                             std::transform_reduce(vec.begin(), vec.end(), vec2.begin(), 0));

    std::cout << std::format("GCD of all elements: {}\n",
                             std::accumulate(vec.begin(), vec.end(), vec.front(),
                                             [](int a, int b) { return std::gcd(a, b); }));

    std::cout << std::format("LCM of all elements: {}\n",
                             std::accumulate(vec.begin(), vec.end(), 1,
                                             [](int a, int b) { return std::lcm(a, b); }));

    std::vector<int> custom_diff(vec.size());
    std::adjacent_difference(vec.begin(), vec.end(), custom_diff.begin(), std::minus<>{});
    print_range("Custom adjacent differences", custom_diff);

    std::vector<int> cumulative_min(vec.size());
    std::partial_sum(vec.begin(), vec.end(), cumulative_min.begin(),
                     [](int a, int b) { return std::min(a, b); });
    print_range("Cumulative minimums", cumulative_min);

    std::vector<int> exclusive_product(vec.size());
    std::exclusive_scan(vec.begin(), vec.end(), exclusive_product.begin(), 1, std::multiplies<>{});
    print_range("Exclusive product scan", exclusive_product);

    std::cout << std::format("Sum of squares: {}\n",
                             std::transform_reduce(vec.begin(), vec.end(), 0, std::plus<>{},
                                                   [](int x) { return x * x; }));

    std::cout << std::format("L2 norm (manual): {:.4f}\n",
                             std::sqrt(static_cast<double>(
                                 std::transform_reduce(vec.begin(), vec.end(), 0, std::plus<>{},
                                                       [](int x) { return x * x; }))));

    std::cout << "\n--- New Additions ---\n";

    std::cout << std::format("mean:    {:.4f}\n", mean(vec));
    std::cout << std::format("variance:{:.4f}\n", variance(vec));
    std::cout << std::format("stddev:  {:.4f}\n", stddev(vec));
    std::cout << std::format("L2 norm: {:.4f}\n", l2_norm(vec));

    std::vector<int> running_max(vec.size());
    std::inclusive_scan(vec.begin(), vec.end(), running_max.begin(),
                        [](int a, int b) { return std::max(a, b); });
    print_range("Running max (inclusive_scan)", running_max);

    std::vector<double> normalised(vec.size());
    const double norm = l2_norm(vec);
    std::ranges::transform(vec, normalised.begin(),
                           [norm](int x) { return static_cast<double>(x) / norm; });
    std::cout << std::format("Normalised (L2=1 check): {:.6f}\n", l2_norm(normalised));

    const auto weighted_sum = std::inner_product(
        vec.begin(), vec.end(), vec2.begin(), 0.0,
        std::plus<double>{},
        [](int a, int b) -> double { return static_cast<double>(a) * b; });
    std::cout << std::format("Weighted sum (vec · vec2): {:.1f}\n", weighted_sum);

    const double cos_sim = weighted_sum / (l2_norm(vec) * l2_norm(vec2));
    std::cout << std::format("Cosine similarity: {:.6f}\n", cos_sim);

    auto fibonacci = std::views::iota(0)
        | std::views::take(10)
        | std::views::transform([](int) { return 1; });
    std::vector<int> fib(10);
    fib[0] = 1; fib[1] = 1;
    for (int i : std::views::iota(2, 10))
        fib[i] = fib[i-1] + fib[i-2];
    print_range("Fibonacci", fib);
    std::cout << std::format("Fibonacci sum: {}\n", range_sum(fib));

    std::vector<int> delta_encoded;
    delta_encoded.reserve(fib.size());
    std::adjacent_difference(fib.begin(), fib.end(), std::back_inserter(delta_encoded));
    print_range("Delta-encoded Fibonacci", delta_encoded);

    std::vector<int> reconstructed(delta_encoded.size());
    std::partial_sum(delta_encoded.begin(), delta_encoded.end(), reconstructed.begin());
    assert(reconstructed == fib);
    std::cout << "Delta decode verified (partial_sum ∘ adjacent_difference = id)\n";

    assert(std::reduce(vec.begin(), vec.end(), 0) == 15);
    assert(std::reduce(vec.begin(), vec.end(), 1, std::multiplies<>{}) == 120);
    assert(inclusive.back() == 15);
    assert(exclusive.front() == 0);
    assert(exclusive_product.front() == 1);

    std::cout << "\nAll assertions passed.\n";
    return 0;
}
