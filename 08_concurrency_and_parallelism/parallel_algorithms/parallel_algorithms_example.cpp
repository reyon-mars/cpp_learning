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
    
    // Sequential
    auto sum_seq = std::reduce(std::execution::seq, data.begin(), data.end(), 0);
    std::cout << "Sequential sum: " << sum_seq << "\n";
    
    // Parallel
    auto sum_par = std::reduce(std::execution::par, data.begin(), data.end(), 0);
    std::cout << "Parallel sum: " << sum_par << "\n";
    
    // Parallel + SIMD
    auto sum_pv = std::reduce(std::execution::par_unseq, data.begin(), data.end(), 0);
    std::cout << "Parallel + SIMD sum: " << sum_pv << "\n";
    
    return 0;
}
