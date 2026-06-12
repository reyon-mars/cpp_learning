#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <functional>
#include <span>
#include <ranges>
#include <cmath>

template <std::ranges::input_range R>
void print_range(std::string_view label, const R& range) {
    std::cout << label << ": ";
    for (const auto& v : range) std::cout << v << ' ';
    std::cout << '\n';
}

int main() {
    constexpr std::array vec  {1, 2, 3, 4, 5};
    constexpr std::array vec2 {2, 3, 4, 5, 6};

    std::cout << "Sum: "
              << std::accumulate(vec.begin(), vec.end(), 0) << '\n';

    std::cout << "Product: "
              << std::accumulate(vec.begin(), vec.end(), 1, std::multiplies<>{}) << '\n';

    std::cout << "Dot product: "
              << std::inner_product(vec.begin(), vec.end(), vec2.begin(), 0) << '\n';

    std::vector<int> partial(vec.size());
    std::partial_sum(vec.begin(), vec.end(), partial.begin());
    print_range("Partial sums", partial);

    std::vector<int> diff(vec.size());
    std::adjacent_difference(vec.begin(), vec.end(), diff.begin());
    print_range("Adjacent differences", diff);

    std::cout << "Subtraction result: "
              << std::accumulate(vec.begin(), vec.end(), 0, std::minus<>{}) << '\n';

    std::cout << "Custom inner product: "
              << std::inner_product(
                     vec.begin(), vec.end(), vec2.begin(), 0,
                     std::plus<>{},
                     [](int a, int b) { return a * b + 1; })
              << '\n';

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

    std::cout << "Reduce sum: "
              << std::reduce(vec.begin(), vec.end(), 0) << '\n';

    std::vector<int> inclusive(vec.size());
    std::inclusive_scan(vec.begin(), vec.end(), inclusive.begin());
    print_range("Inclusive scan", inclusive);

    std::vector<int> exclusive(vec.size());
    std::exclusive_scan(vec.begin(), vec.end(), exclusive.begin(), 0);
    print_range("Exclusive scan", exclusive);

    std::vector<double> running_avg(vec.size());
    std::inclusive_scan(vec.begin(), vec.end(), running_avg.begin(), std::plus<double>{});
    for (std::size_t i = 0; i < running_avg.size(); ++i) running_avg[i] /= static_cast<double>(i + 1);
    print_range("Running average", running_avg);

    std::cout << "Transform reduce result: "
              << std::transform_reduce(vec.begin(), vec.end(), vec2.begin(), 0) << '\n';

    std::cout << "GCD of all elements: "
              << std::accumulate(vec.begin(), vec.end(), vec.front(),
                                 [](int a, int b) { return std::gcd(a, b); }) << '\n';

    std::cout << "LCM of all elements: "
              << std::accumulate(vec.begin(), vec.end(), 1,
                                 [](int a, int b) { return std::lcm(a, b); }) << '\n';

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

    std::cout << "Sum of squares: "
              << std::transform_reduce(vec.begin(), vec.end(), 0, std::plus<>{},
                                       [](int x) { return x * x; }) << '\n';

    std::cout << "L2 norm: "
              << std::sqrt(static_cast<double>(
                     std::transform_reduce(vec.begin(), vec.end(), 0, std::plus<>{},
                                           [](int x) { return x * x; })))
              << '\n';

    return 0;
}
