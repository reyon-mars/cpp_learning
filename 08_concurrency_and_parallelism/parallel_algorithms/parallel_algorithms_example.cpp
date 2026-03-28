// Parallel Algorithms Exercise
// std::execution policies

#include <iostream>
#include <vector>
#include <algorithm>
#include <execution>
#include <numeric>

// ---------------- SMALL ADDITIONS ----------------

// Check if all elements satisfy condition (parallel)
bool all_positive(const std::vector<int>& v) {
    return std::all_of(std::execution::par, v.begin(), v.end(),
                       [](int x) { return x > 0; });
}

// Count even numbers (parallel)
int count_even(const std::vector<int>& v) {
    return std::count_if(std::execution::par, v.begin(), v.end(),
                         [](int x) { return x % 2 == 0; });
}

// ---------------- MAIN ----------------

int main() {
    std::vector<int> data(1000);
    std::iota(data.begin(), data.end(), 1);

    // Sequential reduce
    auto sum_seq = std::reduce(std::execution::seq, data.begin(), data.end(), 0);
    std::cout << "Sequential sum: " << sum_seq << "\n";

    // Parallel reduce
    auto sum_par = std::reduce(std::execution::par, data.begin(), data.end(), 0);
    std::cout << "Parallel sum: " << sum_par << "\n";

    // Parallel + SIMD
    auto sum_pv = std::reduce(std::execution::par_unseq, data.begin(), data.end(), 0);
    std::cout << "Parallel + SIMD sum: " << sum_pv << "\n";


    // ---------------- Parallel transform ----------------
    std::vector<int> squared(data.size());

    std::transform(std::execution::par,
                   data.begin(),
                   data.end(),
                   squared.begin(),
                   [](int x) { return x * x; });

    std::cout << "First 5 squared values: ";
    for (int i = 0; i < 5; ++i) {
        std::cout << squared[i] << " ";
    }
    std::cout << "\n";


    // ---------------- Parallel for_each ----------------
    std::for_each(std::execution::par,
                  data.begin(),
                  data.end(),
                  [](int& x) { x += 1; });

    std::cout << "First 5 incremented values: ";
    for (int i = 0; i < 5; ++i) {
        std::cout << data[i] << " ";
    }
    std::cout << "\n";


    // ---------------- Parallel sort ----------------
    std::vector<int> unsorted = {9, 4, 7, 1, 3, 6, 8, 2, 5};

    std::sort(std::execution::par, unsorted.begin(), unsorted.end());

    std::cout << "Sorted values: ";
    for (int v : unsorted) {
        std::cout << v << " ";
    }
    std::cout << "\n";


    // ---------------- ADDED USAGE ----------------

    // Check all positive
    std::cout << "All elements positive? "
              << (all_positive(data) ? "Yes" : "No") << "\n";

    // Count even numbers
    std::cout << "Even numbers count: "
              << count_even(data) << "\n";

    // Parallel min/max
    auto minmax = std::minmax_element(std::execution::par,
                                     data.begin(), data.end());

    std::cout << "Min: " << *minmax.first
              << ", Max: " << *minmax.second << "\n";

    // Parallel copy
    std::vector<int> copied(data.size());
    std::copy(std::execution::par,
              data.begin(), data.end(),
              copied.begin());

    std::cout << "First 5 copied values: ";
    for (int i = 0; i < 5; ++i) {
        std::cout << copied[i] << " ";
    }
    std::cout << "\n";

    return 0;
}
