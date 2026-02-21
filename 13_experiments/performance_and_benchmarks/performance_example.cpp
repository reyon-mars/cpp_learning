// Performance and Benchmarks Experiment
// Measuring execution time, cache behavior, optimization comparisons

#include <iostream>
#include <chrono>
#include <vector>

int main() {
    // Example: Vector access patterns
    std::vector<int> vec(10000);
    for (int i = 0; i < vec.size(); ++i) {
        vec[i] = i;
    }
    
    auto start = std::chrono::high_resolution_clock::now();
    
    int sum = 0;
    for (int v : vec) {
        sum += v;
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "Sum: " << sum << "\n";
    std::cout << "Time: " << duration.count() << " microseconds\n";
    
    return 0;
}
