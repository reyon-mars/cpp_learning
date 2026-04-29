// Numeric Algorithms Exercise
// accumulate, inner_product, adjacent_difference, partial_sum

#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    
    // accumulate (sum)
    int sum = std::accumulate(vec.begin(), vec.end(), 0);
    std::cout << "Sum: " << sum << "\n";
    
    // accumulate with custom operation
    int product = std::accumulate(vec.begin(), vec.end(), 1,
                                  [](int a, int b) { return a * b; });
    std::cout << "Product: " << product << "\n";
    
    // inner_product
    std::vector<int> vec2 = {2, 3, 4, 5, 6};
    int dot_product = std::inner_product(vec.begin(), vec.end(), 
                                         vec2.begin(), 0);
    std::cout << "Dot product: " << dot_product << "\n";

    // partial_sum
    std::vector<int> partial(vec.size());
    std::partial_sum(vec.begin(), vec.end(), partial.begin());
    std::cout << "Partial sums: ";
    for (int v : partial) {
        std::cout << v << " ";
    }
    std::cout << "\n";

    // adjacent_difference
    std::vector<int> diff(vec.size());
    std::adjacent_difference(vec.begin(), vec.end(), diff.begin());
    std::cout << "Adjacent differences: ";
    for (int v : diff) {
        std::cout << v << " ";
    }
    std::cout << "\n";

    // ---------------- SMALL ADDITIONS ----------------

    // accumulate with subtraction
    int subtract_all = std::accumulate(vec.begin(), vec.end(), 0,
                                       [](int a, int b) { return a - b; });
    std::cout << "Subtraction result: " << subtract_all << "\n";

    // inner_product with custom operations
    int custom_inner = std::inner_product(
        vec.begin(), vec.end(), vec2.begin(), 0,
        std::plus<>(),
        [](int a, int b) { return a * b + 1; }
    );
    std::cout << "Custom inner product: " << custom_inner << "\n";

    // partial_sum with multiplication
    std::vector<int> partial_product(vec.size());
    std::partial_sum(vec.begin(), vec.end(), partial_product.begin(),
                     [](int a, int b) { return a * b; });

    std::cout << "Partial products: ";
    for (int v : partial_product) {
        std::cout << v << " ";
    }
    std::cout << "\n";

    // adjacent_difference with multiplication
    std::vector<int> diff_product(vec.size());
    std::adjacent_difference(vec.begin(), vec.end(), diff_product.begin(),
                             [](int a, int b) { return a * b; });

    std::cout << "Adjacent products: ";
    for (int v : diff_product) {
        std::cout << v << " ";
    }
    std::cout << "\n";

    // prefix max using partial_sum
    std::vector<int> prefix_max(vec.size());
    std::partial_sum(vec.begin(), vec.end(), prefix_max.begin(),
                     [](int a, int b) { return std::max(a, b); });

    std::cout << "Prefix max: ";
    for (int v : prefix_max) {
        std::cout << v << " ";
    }
    std::cout << "\n";

    // 🔹 NEW: std::reduce (C++17)
    int reduce_sum = std::reduce(vec.begin(), vec.end(), 0);
    std::cout << "Reduce sum: " << reduce_sum << "\n";

    // 🔹 NEW: inclusive_scan (like partial_sum)
    std::vector<int> inclusive(vec.size());
    std::inclusive_scan(vec.begin(), vec.end(), inclusive.begin());

    std::cout << "Inclusive scan: ";
    for (int v : inclusive) {
        std::cout << v << " ";
    }
    std::cout << "\n";

    // 🔹 NEW: exclusive_scan
    std::vector<int> exclusive(vec.size());
    std::exclusive_scan(vec.begin(), vec.end(), exclusive.begin(), 0);

    std::cout << "Exclusive scan: ";
    for (int v : exclusive) {
        std::cout << v << " ";
    }
    std::cout << "\n";

    // 🔹 NEW: running average
    std::vector<double> running_avg(vec.size());
    std::partial_sum(vec.begin(), vec.end(), running_avg.begin(),
                     [](double a, double b) { return a + b; });

    for (size_t i = 0; i < running_avg.size(); ++i) {
        running_avg[i] /= (i + 1);
    }

    std::cout << "Running average: ";
    for (double v : running_avg) {
        std::cout << v << " ";
    }
    std::cout << "\n";

    return 0;
}
