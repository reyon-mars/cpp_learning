// Performance and Benchmarks Experiment
// Measuring execution time, cache behavior, optimization comparisons

#include <iostream>
#include <chrono>
#include <vector>

// ✅ ADDED
#include <numeric>   // for std::accumulate
#include <algorithm> // ✅ ADDED
#include <functional> // ✅ ADDED

// -------- NEW ADDITIONS --------

// Prevent optimization
volatile int sink = 0;

// Run benchmark multiple times
template<typename Func>
long long benchmark(Func f, int runs = 5) {
    long long total = 0;
    for (int i = 0; i < runs; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        sink = f();
        auto end = std::chrono::high_resolution_clock::now();

        total += std::chrono::duration_cast<std::chrono::microseconds>(
                     end - start).count();
    }
    return total / runs;
}

// Warm-up function
void warmup() {
    for (volatile int i = 0; i < 1000000; ++i);
}

// ✅ ADDED: simple result printer
void print_result(const std::string& label, long long time) {
    std::cout << label << ": " << time << " microseconds\n";
}

// ✅ ADDED: compare two timings
void compare(const std::string& name, long long a, long long b) {
    std::cout << name << " faster: "
              << (a < b ? "First" : "Second") << "\n";
}

// --------------------------------

int main() {

    const int N = 10000;

    warmup(); // ✅ ADDED

    // Example: Vector access patterns
    std::vector<int> vec;
    vec.reserve(N);  // small optimization

    for (int i = 0; i < N; ++i) {
        vec.push_back(i);
    }

    std::cout << "Vector size: " << vec.size() << "\n";

    // -------- Range-based loop benchmark --------
    auto duration1 = benchmark([&]() {
        int sum = 0;
        for (int v : vec) sum += v;
        return sum;
    });

    // -------- Index loop benchmark --------
    auto duration2 = benchmark([&]() {
        int sum = 0;
        for (size_t i = 0; i < vec.size(); ++i) sum += vec[i];
        return sum;
    });

    // ✅ ADDED: Iterator loop benchmark
    auto duration3 = benchmark([&]() {
        int sum = 0;
        for (auto it = vec.begin(); it != vec.end(); ++it) sum += *it;
        return sum;
    });

    // ✅ ADDED: STL accumulate benchmark
    auto duration4 = benchmark([&]() {
        return std::accumulate(vec.begin(), vec.end(), 0);
    });

    // -------- Results --------
    print_result("Time (range loop)", duration1);
    print_result("Time (index loop)", duration2);
    print_result("Time (iterator loop)", duration3);
    print_result("Time (accumulate)", duration4);

    compare("Range vs Index", duration1, duration2);

    // ----------------------------------------------------
    // ✅ ADDED: Cache locality experiment (2D array)
    const int SIZE = 300;
    std::vector<std::vector<int>> matrix(SIZE, std::vector<int>(SIZE, 1));

    auto duration5 = benchmark([&]() {
        int sum = 0;
        for (int i = 0; i < SIZE; ++i)
            for (int j = 0; j < SIZE; ++j)
                sum += matrix[i][j];
        return sum;
    });

    auto duration6 = benchmark([&]() {
        int sum = 0;
        for (int j = 0; j < SIZE; ++j)
            for (int i = 0; i < SIZE; ++i)
                sum += matrix[i][j];
        return sum;
    });

    std::cout << "\nCache Locality Test (2D vector):\n";
    print_result("Row-wise Time", duration5);
    print_result("Column-wise Time", duration6);

    // ----------------------------------------------------
    // ✅ NEW: Contiguous memory vs non-contiguous

    std::vector<int> flat(SIZE * SIZE, 1);

    auto duration7 = benchmark([&]() {
        int sum = 0;
        for (int i = 0; i < SIZE; ++i)
            for (int j = 0; j < SIZE; ++j)
                sum += flat[i * SIZE + j];   // row-major
        return sum;
    });

    auto duration8 = benchmark([&]() {
        int sum = 0;
        for (int j = 0; j < SIZE; ++j)
            for (int i = 0; i < SIZE; ++i)
                sum += flat[i * SIZE + j];   // column-like
        return sum;
    });

    std::cout << "\nCache Locality Test (1D contiguous array):\n";
    print_result("Row-wise Time", duration7);
    print_result("Column-wise Time", duration8);

    // ----------------------------------------------------
    // ✅ VERY SMALL EXTRA ADDITIONS

    // find max element benchmark
    auto duration9 = benchmark([&]() {
        return *std::max_element(vec.begin(), vec.end());
    });

    print_result("\nMax element (std::max_element)", duration9);

    // simple sanity check
    std::cout << "Sink value (avoid optimization): " << sink << "\n";

    // ----------------------------------------------------

    return 0;
}
