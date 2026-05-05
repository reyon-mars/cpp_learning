#include <iostream>
#include <array>
#include <type_traits>   // ✅ ADDED

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

// Template metaprogramming Fibonacci
template<int N>
struct Fib {
    static constexpr int value = Fib<N-1>::value + Fib<N-2>::value;
};

template<>
struct Fib<1> { static constexpr int value = 1; };

template<>
struct Fib<0> { static constexpr int value = 0; };

// Compile-time power
constexpr int power_ct(int base, int exp) {
    return (exp == 0) ? 1 : base * power_ct(base, exp - 1);
}

// Compile-time prime check
constexpr bool is_prime_ct(int n, int i = 2) {
    return (n <= 2) ? (n == 2) :
           (n % i == 0) ? false :
           (i * i > n) ? true :
           is_prime_ct(n, i + 1);
}

// ----------- NEW ADDITIONS -----------

// Compile-time max of array
template<std::size_t N>
constexpr int max_array_ct(const std::array<int, N>& arr) {
    int max = arr[0];
    for (std::size_t i = 1; i < N; ++i) {
        if (arr[i] > max) max = arr[i];
    }
    return max;
}

// Compile-time even check
constexpr bool is_even_ct(int n) {
    return n % 2 == 0;
}

// Type trait demo
template<typename T>
constexpr bool is_integral_ct() {
    return std::is_integral<T>::value;
}

// Compile-time sum from 1..N
constexpr int sum_n_ct(int n) {
    return (n <= 0) ? 0 : n + sum_n_ct(n - 1);
}

// ===== VERY SMALL NEW ADDITIONS =====

// Compile-time min of array
template<std::size_t N>
constexpr int min_array_ct(const std::array<int, N>& arr) {
    int min = arr[0];
    for (std::size_t i = 1; i < N; ++i) {
        if (arr[i] < min) min = arr[i];
    }
    return min;
}

// Compile-time absolute value
constexpr int abs_ct(int x) {
    return x < 0 ? -x : x;
}

// Compile-time gcd (Euclidean algorithm)
constexpr int gcd_ct(int a, int b) {
    return (b == 0) ? a : gcd_ct(b, a % b);
}

// Compile-time check for odd
constexpr bool is_odd_ct(int n) {
    return !is_even_ct(n);
}

// ====================================

// ------------------------------------

int main() {

    constexpr int fib15 = fibonacci_ct(15);
    constexpr auto sequence = generate_sequence();

    // compile-time factorial
    constexpr int fact6 = factorial_ct(6);

    // compile-time sum
    constexpr int seq_sum = sum_array_ct(sequence);

    // More compile-time values
    constexpr int fib10_tm = Fib<10>::value;
    constexpr int pow_val = power_ct(2, 8);
    constexpr bool prime_check = is_prime_ct(29);

    // -------- NEW USAGE --------
    constexpr int max_val = max_array_ct(sequence);
    constexpr bool even_check = is_even_ct(42);
    constexpr int sum10 = sum_n_ct(10);
    constexpr bool is_int = is_integral_ct<int>();

    // ===== NEW USAGE =====
    constexpr int min_val = min_array_ct(sequence);
    constexpr int abs_val = abs_ct(-25);
    constexpr int gcd_val = gcd_ct(48, 18);
    constexpr bool odd_check = is_odd_ct(7);
    // =====================

    // --------------------------

    // compile-time checks
    static_assert(fibonacci_ct(10) == 55, "Fib calculation incorrect");
    static_assert(factorial_ct(5) == 120, "Factorial incorrect");

    static_assert(Fib<10>::value == 55, "Template Fib incorrect");
    static_assert(power_ct(2, 5) == 32, "Power incorrect");
    static_assert(is_prime_ct(7), "Prime check failed");

    // -------- NEW ASSERTS --------
    static_assert(max_array_ct(generate_sequence()) == 81, "Max incorrect");
    static_assert(is_even_ct(10), "Even check failed");
    static_assert(sum_n_ct(5) == 15, "Sum N incorrect");
    static_assert(is_integral_ct<int>(), "Type trait failed");

    // ===== NEW ASSERTS =====
    static_assert(min_array_ct(generate_sequence()) == 0, "Min incorrect");
    static_assert(abs_ct(-10) == 10, "Abs failed");
    static_assert(gcd_ct(48, 18) == 6, "GCD failed");
    static_assert(is_odd_ct(3), "Odd check failed");
    // =======================

    // ----------------------------

    std::cout << "Fib(15) at compile-time: " << fib15 << "\n";

    std::cout << "Sequence: ";
    for (int v : sequence) {
        std::cout << v << " ";
    }
    std::cout << "\n";

    std::cout << "Factorial(6): " << fact6 << "\n";
    std::cout << "Sum of sequence: " << seq_sum << "\n";

    std::cout << "Fib<10> (template): " << fib10_tm << "\n";
    std::cout << "2^8 (compile-time): " << pow_val << "\n";
    std::cout << "Is 29 prime? " << (prime_check ? "Yes" : "No") << "\n";

    // -------- NEW OUTPUT --------
    std::cout << "Max in sequence: " << max_val << "\n";
    std::cout << "Is 42 even? " << (even_check ? "Yes" : "No") << "\n";
    std::cout << "Sum 1..10: " << sum10 << "\n";
    std::cout << "Is int integral? " << (is_int ? "Yes" : "No") << "\n";

    // ===== NEW OUTPUT =====
    std::cout << "Min in sequence: " << min_val << "\n";
    std::cout << "Absolute(-25): " << abs_val << "\n";
    std::cout << "GCD(48,18): " << gcd_val << "\n";
    std::cout << "Is 7 odd? " << (odd_check ? "Yes" : "No") << "\n";
    // ======================

    // ---------------------------

    return 0;
}
