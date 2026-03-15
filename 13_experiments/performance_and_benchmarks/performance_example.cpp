// Performance and Benchmarks Experiment
// Measuring execution time, cache behavior, optimization comparisons

#include <iostream>
#include <chrono>
#include <vector>

int main() {

    const int N = 10000;

    // Example: Vector access patterns
    std::vector<int> vec;
    vec.reserve(N);  // small optimization

    for (int i = 0; i < N; ++i) {
        vec.push_back(i);
    }

    std::cout << "Vector size: " << vec.size() << "\n";

    // -------- Range-based loop benchmark --------
    auto start1 = std::chrono::high_resolution_clock::now();

    int sum1 = 0;
    for (int v : vec) {
        sum1 += v;
    }

    auto end1 = std::chrono::high_resolution_clock::now();
    auto duration1 =
        std::chrono::duration_cast<std::chrono::microseconds>(end1 - start1);

    // -------- Index loop benchmark --------
    auto start2 = std::chrono::high_resolution_clock::now();

    int sum2 = 0;
    for (size_t i = 0; i < vec.size(); ++i) {
        sum2 += vec[i];
    }

    auto end2 = std::chrono::high_resolution_clock::now();
    auto duration2 =
        std::chrono::duration_cast<std::chrono::microseconds>(end2 - start2);

    // -------- Results --------
    std::cout << "Sum (range loop): " << sum1 << "\n";
    std::cout << "Time (range loop): "
              << duration1.count() << " microseconds\n";

    std::cout << "Sum (index loop): " << sum2 << "\n";
    std::cout << "Time (index loop): "
              << duration2.count() << " microseconds\n";

    return 0;
}
