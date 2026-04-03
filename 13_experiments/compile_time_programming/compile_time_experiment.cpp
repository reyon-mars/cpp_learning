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

// ✅ ADDED: Template metaprogramming Fibonacci
template<int N>
struct Fib {
    static constexpr int value = Fib<N-1>::value + Fib<N-2>::value;
};

template<>
struct Fib<1> { static constexpr int value = 1; };

template<>
struct Fib<0> { static constexpr int value = 0; };

// ✅ ADDED: Compile-time power
constexpr int power_ct(int base, int exp) {
    return (exp == 0) ? 1 : base * power_ct(base, exp - 1);
}

// ✅ ADDED: Compile-time prime check
constexpr bool is_prime_ct(int n, int i = 2) {
    return (n <= 2) ? (n == 2) :
           (n % i == 0) ? false :
           (i * i > n) ? true :
           is_prime_ct(n, i + 1);
}

// -----------------------------------------------

int main() {

    constexpr int fib15 = fibonacci_ct(15);
    constexpr auto sequence = generate_sequence();

    // compile-time factorial
    constexpr int fact6 = factorial_ct(6);

    // compile-time sum
    constexpr int seq_sum = sum_array_ct(sequence);

    // ✅ ADDED: More compile-time values
    constexpr int fib10_tm = Fib<10>::value;
    constexpr int pow_val = power_ct(2, 8);
    constexpr bool prime_check = is_prime_ct(29);

    // compile-time checks
    static_assert(fibonacci_ct(10) == 55, "Fib calculation incorrect");
    static_assert(factorial_ct(5) == 120, "Factorial incorrect");

    // ✅ ADDED: Extra static checks
    static_assert(Fib<10>::value == 55, "Template Fib incorrect");
    static_assert(power_ct(2, 5) == 32, "Power incorrect");
    static_assert(is_prime_ct(7), "Prime check failed");

    std::cout << "Fib(15) at compile-time: " << fib15 << "\n";

    std::cout << "Sequence: ";
    for (int v : sequence) {
        std::cout << v << " ";
    }
    std::cout << "\n";

    std::cout << "Factorial(6): " << fact6 << "\n";
    std::cout << "Sum of sequence: " << seq_sum << "\n";

    // ✅ ADDED: Output new results
    std::cout << "Fib<10> (template): " << fib10_tm << "\n";
    std::cout << "2^8 (compile-time): " << pow_val << "\n";
    std::cout << "Is 29 prime? " << (prime_check ? "Yes" : "No") << "\n";

    return 0;
}
