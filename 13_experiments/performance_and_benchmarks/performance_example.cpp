#include <iostream>
#include <chrono>
#include <vector>
#include <numeric>
#include <algorithm>
#include <functional>
#include <array>
#include <ranges>
#include <format>
#include <string_view>
#include <concepts>

namespace {
volatile int sink = 0;
}

template <std::invocable F>
[[nodiscard]] std::chrono::microseconds benchmark(F&& f, int runs = 5) {
    using namespace std::chrono;
    microseconds total{0};

    for (int i = 0; i < runs; ++i) {
        const auto start = high_resolution_clock::now();
        sink = static_cast<int>(std::forward<F>(f)());
        const auto end = high_resolution_clock::now();
        total += duration_cast<microseconds>(end - start);
    }

    return total / runs;
}

void warmup() noexcept {
    volatile int x = 0;
    for (int i = 0; i < 1'000'000; ++i) x = i;
    (void)x;
}

void print_result(std::string_view label, std::chrono::microseconds time) {
    std::cout << std::format("{}: {} microseconds\n", label, time.count());
}

void compare(std::string_view name, std::chrono::microseconds a, std::chrono::microseconds b) {
    std::cout << std::format("{} faster: {}\n", name, a < b ? "First" : "Second");
}

void divider() { std::cout << "----------------------------------\n"; }

template <std::size_t N>
[[nodiscard]] std::chrono::microseconds average_time(
    const std::array<std::chrono::microseconds, N>& times) noexcept {
    return std::reduce(times.begin(), times.end(), std::chrono::microseconds{0}) /
           static_cast<long long>(N);
}

template <std::size_t N>
void show_best(const std::array<std::chrono::microseconds, N>& times) {
    const auto best = *std::ranges::min_element(times);
    std::cout << std::format("Best benchmark time: {} microseconds\n", best.count());
}

int main() {
    constexpr int n = 10000;

    warmup();

    std::vector<int> vec;
    vec.reserve(n);
    std::ranges::copy(std::views::iota(0, n), std::back_inserter(vec));

    std::cout << std::format("Vector size: {}\n", vec.size());

    const auto duration_range_loop = benchmark([&] {
        int sum = 0;
        for (const int v : vec) sum += v;
        return sum;
    });

    const auto duration_index_loop = benchmark([&] {
        int sum = 0;
        for (std::size_t i = 0; i < vec.size(); ++i) sum += vec[i];
        return sum;
    });

    const auto duration_iterator_loop = benchmark([&] {
        int sum = 0;
        for (auto it = vec.begin(); it != vec.end(); ++it) sum += *it;
        return sum;
    });

    const auto duration_accumulate = benchmark([&] {
        return std::reduce(vec.begin(), vec.end(), 0);
    });

    print_result("Time (range loop)",    duration_range_loop);
    print_result("Time (index loop)",    duration_index_loop);
    print_result("Time (iterator loop)", duration_iterator_loop);
    print_result("Time (accumulate)",    duration_accumulate);

    compare("Range vs Index", duration_range_loop, duration_index_loop);

    const std::array loop_times{
        duration_range_loop, duration_index_loop,
        duration_iterator_loop, duration_accumulate,
    };

    std::cout << std::format("Average loop benchmark: {} microseconds\n",
                             average_time(loop_times).count());
    show_best(loop_times);

    divider();

    constexpr int size = 300;

    std::vector<std::vector<int>> matrix(size, std::vector<int>(size, 1));

    const auto duration_row_major_2d = benchmark([&] {
        int sum = 0;
        for (int i = 0; i < size; ++i)
            for (int j = 0; j < size; ++j)
                sum += matrix[i][j];
        return sum;
    });

    const auto duration_col_major_2d = benchmark([&] {
        int sum = 0;
        for (int j = 0; j < size; ++j)
            for (int i = 0; i < size; ++i)
                sum += matrix[i][j];
        return sum;
    });

    std::cout << "\nCache Locality Test (2D vector):\n";
    print_result("Row-wise Time",    duration_row_major_2d);
    print_result("Column-wise Time", duration_col_major_2d);
    compare("2D Row vs Column", duration_row_major_2d, duration_col_major_2d);

    divider();

    std::vector<int> flat(static_cast<std::size_t>(size) * size, 1);

    const auto duration_row_major_1d = benchmark([&] {
        int sum = 0;
        for (int i = 0; i < size; ++i)
            for (int j = 0; j < size; ++j)
                sum += flat[i * size + j];
        return sum;
    });

    const auto duration_col_major_1d = benchmark([&] {
        int sum = 0;
        for (int j = 0; j < size; ++j)
            for (int i = 0; i < size; ++i)
                sum += flat[i * size + j];
        return sum;
    });

    std::cout << "\nCache Locality Test (1D contiguous array):\n";
    print_result("Row-wise Time",    duration_row_major_1d);
    print_result("Column-wise Time", duration_col_major_1d);
    compare("1D Row vs Column", duration_row_major_1d, duration_col_major_1d);

    divider();

    const auto duration_max_element = benchmark([&] {
        return *std::ranges::max_element(vec);
    });
    print_result("Max element (std::ranges::max_element)", duration_max_element);

    const auto duration_min_element = benchmark([&] {
        return *std::ranges::min_element(vec);
    });
    print_result("Min element (std::ranges::min_element)", duration_min_element);

    const auto duration_count_even = benchmark([&] {
        return std::ranges::count_if(vec, [](int x) { return x % 2 == 0; });
    });
    print_result("Count even numbers", duration_count_even);

    std::cout << std::format("\nSink value (avoid optimization): {}\n", sink);

    return 0;
}
