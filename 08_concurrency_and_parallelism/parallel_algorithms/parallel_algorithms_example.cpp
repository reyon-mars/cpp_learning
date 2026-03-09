// Parallel Algorithms Exercise
// std::execution policies

#include <iostream>
#include <vector>
#include <algorithm>
#include <execution>
#include <numeric>

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

    return 0;
}
