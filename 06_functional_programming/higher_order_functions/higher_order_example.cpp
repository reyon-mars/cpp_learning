// Higher-Order Functions Exercise
// Functions that work with other functions

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <functional>

int add(int a, int b) { return a + b; }
int multiply(int a, int b) { return a * b; }

// ----------------------------------
// Higher-order function: applies a binary operation
// ----------------------------------
template<typename BinOp>
int reduce(const std::vector<int>& values, BinOp op) {
    int result = values[0];
    for (size_t i = 1; i < values.size(); ++i) {
        result = op(result, values[i]);
    }
    return result;
}

// ----------------------------------
// Generic reduce (type-independent)
// ----------------------------------
template<typename T, typename BinOp>
T reduce_generic(const std::vector<T>& values, BinOp op) {
    T result = values[0];
    for (size_t i = 1; i < values.size(); ++i) {
        result = op(result, values[i]);
    }
    return result;
}

// ----------------------------------
// Function returning a function
// ----------------------------------
std::function<int(int)> make_multiplier(int factor) {
    return [factor](int x) {
        return x * factor;
    };
}

// ----------------------------------
// Main
// ----------------------------------
int main() {

    std::vector<int> values = {1, 2, 3, 4, 5};

    int sum = reduce(values, add);
    int product = reduce(values, multiply);

    std::cout << "Sum: " << sum << "\n";
    std::cout << "Product: " << product << "\n";

    // Using lambda
    int max_val = reduce(values,
        [](int a, int b) { return std::max(a, b); });

    std::cout << "Max: " << max_val << "\n";

    // Generic reduce
    double avg = reduce_generic<double>(
        {1.0, 2.0, 3.0},
        [](double a, double b) { return a + b; }
    ) / 3.0;

    std::cout << "Average: " << avg << "\n";

    // std::accumulate comparison
    int std_sum = std::accumulate(
        values.begin(), values.end(), 0);

    std::cout << "std::accumulate sum: "
              << std_sum << "\n";

    // Transform (higher-order)
    std::vector<int> doubled(values.size());
    std::transform(values.begin(),
                   values.end(),
                   doubled.begin(),
                   make_multiplier(2));

    std::cout << "Doubled values: ";
    for (int v : doubled)
        std::cout << v << " ";
    std::cout << "\n";

    return 0;
}
