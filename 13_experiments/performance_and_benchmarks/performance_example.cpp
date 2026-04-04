// Performance and Benchmarks Experiment
// Measuring execution time, cache behavior, optimization comparisons

#include <iostream>
#include <chrono>
#include <vector>

// ✅ ADDED
#include <numeric>   // for std::accumulate

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

    // ✅ ADDED: Iterator loop benchmark
    auto start3 = std::chrono::high_resolution_clock::now();

    int sum3 = 0;
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        sum3 += *it;
    }

    auto end3 = std::chrono::high_resolution_clock::now();
    auto duration3 =
        std::chrono::duration_cast<std::chrono::microseconds>(end3 - start3);

    // ✅ ADDED: STL accumulate benchmark
    auto start4 = std::chrono::high_resolution_clock::now();

    int sum4 = std::accumulate(vec.begin(), vec.end(), 0);

    auto end4 = std::chrono::high_resolution_clock::now();
    auto duration4 =
        std::chrono::duration_cast<std::chrono::microseconds>(end4 - start4);

    // -------- Results --------
    std::cout << "Sum (range loop): " << sum1 << "\n";
    std::cout << "Time (range loop): "
              << duration1.count() << " microseconds\n";

    std::cout << "Sum (index loop): " << sum2 << "\n";
    std::cout << "Time (index loop): "
              << duration2.count() << " microseconds\n";

    // ✅ ADDED
    std::cout << "Sum (iterator loop): " << sum3 << "\n";
    std::cout << "Time (iterator loop): "
              << duration3.count() << " microseconds\n";

    std::cout << "Sum (accumulate): " << sum4 << "\n";
    std::cout << "Time (accumulate): "
              << duration4.count() << " microseconds\n";

    // ----------------------------------------------------
    // ✅ ADDED: Cache locality experiment (2D array)
    const int SIZE = 300;
    std::vector<std::vector<int>> matrix(SIZE, std::vector<int>(SIZE, 1));

    // Row-wise (cache-friendly)
    auto start5 = std::chrono::high_resolution_clock::now();

    int sum_row = 0;
    for (int i = 0; i < SIZE; ++i)
        for (int j = 0; j < SIZE; ++j)
            sum_row += matrix[i][j];

    auto end5 = std::chrono::high_resolution_clock::now();
    auto duration5 =
        std::chrono::duration_cast<std::chrono::microseconds>(end5 - start5);

    // Column-wise (cache-unfriendly)
    auto start6 = std::chrono::high_resolution_clock::now();

    int sum_col = 0;
    for (int j = 0; j < SIZE; ++j)
        for (int i = 0; i < SIZE; ++i)
            sum_col += matrix[i][j];

    auto end6 = std::chrono::high_resolution_clock::now();
    auto duration6 =
        std::chrono::duration_cast<std::chrono::microseconds>(end6 - start6);

    std::cout << "\nCache Locality Test:\n";
    std::cout << "Row-wise sum: " << sum_row 
              << " Time: " << duration5.count() << " microseconds\n";

    std::cout << "Column-wise sum: " << sum_col 
              << " Time: " << duration6.count() << " microseconds\n";

    return 0;
}
