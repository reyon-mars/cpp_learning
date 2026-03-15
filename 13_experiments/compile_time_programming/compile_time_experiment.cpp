// Compile-time Programming Experiment
// Exploring constexpr limits and compile-time computation

#include <iostream>
#include <array>

constexpr int fibonacci_ct(int n) {
    return n <= 1 ? n : fibonacci_ct(n - 1) + fibonacci_ct(n - 2);
}

constexpr std::array<int, 10> generate_sequence() {
    std::array<int, 10> seq{};
    for (int i = 0; i < 10; ++i) {
        seq[i] = i * i;
    }
    return seq;
}

// ---- Small additional compile-time helpers ----

// Compile-time factorial
constexpr int factorial_ct(int n) {
    return (n <= 1) ? 1 : n * factorial_ct(n - 1);
}

// Compile-time array sum
constexpr int sum_array_ct(const std::array<int,10>& arr) {
    int sum = 0;
    for (auto v : arr) sum += v;
    return sum;
}

// -----------------------------------------------

int main() {

    constexpr int fib15 = fibonacci_ct(15);
    constexpr auto sequence = generate_sequence();

    // compile-time factorial
    constexpr int fact6 = factorial_ct(6);

    // compile-time sum
    constexpr int seq_sum = sum_array_ct(sequence);

    // compile-time checks
    static_assert(fibonacci_ct(10) == 55, "Fib calculation incorrect");
    static_assert(factorial_ct(5) == 120, "Factorial incorrect");

    std::cout << "Fib(15) at compile-time: " << fib15 << "\n";

    std::cout << "Sequence: ";
    for (int v : sequence) {
        std::cout << v << " ";
    }
    std::cout << "\n";

    std::cout << "Factorial(6): " << fact6 << "\n";
    std::cout << "Sum of sequence: " << seq_sum << "\n";

    return 0;
}
