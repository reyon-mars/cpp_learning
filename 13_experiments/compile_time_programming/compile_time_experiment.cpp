#include <iostream>
#include <array>
#include <type_traits>
#include <algorithm>
#include <numeric>

[[nodiscard]] constexpr int fibonacci_ct(int n) noexcept {
    return n <= 1 ? n : fibonacci_ct(n-1) + fibonacci_ct(n-2);
}

[[nodiscard]] constexpr int factorial_ct(int n) noexcept {
    return n <= 1 ? 1 : n * factorial_ct(n-1);
}

[[nodiscard]] constexpr int power_ct(int base, int exp) noexcept {
    return exp == 0 ? 1 : base * power_ct(base, exp-1);
}

[[nodiscard]] constexpr bool is_prime_ct(int n, int i = 2) noexcept {
    return n <= 2    ? (n == 2)    :
           n % i == 0 ? false      :
           i * i > n  ? true       :
                        is_prime_ct(n, i+1);
}

[[nodiscard]] constexpr bool is_even_ct (int n)          noexcept { return n % 2 == 0; }
[[nodiscard]] constexpr bool is_odd_ct  (int n)          noexcept { return n % 2 != 0; }
[[nodiscard]] constexpr int  abs_ct     (int x)          noexcept { return x < 0 ? -x : x; }
[[nodiscard]] constexpr int  cube_ct    (int x)          noexcept { return x * x * x; }
[[nodiscard]] constexpr int  sum_n_ct   (int n)          noexcept { return n <= 0 ? 0 : n + sum_n_ct(n-1); }
[[nodiscard]] constexpr int  clamp_ct   (int v, int lo, int hi) noexcept { return v < lo ? lo : v > hi ? hi : v; }
[[nodiscard]] constexpr int  gcd_ct     (int a, int b)   noexcept { return b == 0 ? a : gcd_ct(b, a % b); }

template<typename T>
inline constexpr bool is_integral_ct_v = std::is_integral_v<T>;

template<int N>
struct Fib {
    static constexpr int value = Fib<N-1>::value + Fib<N-2>::value;
};
template<> struct Fib<1> { static constexpr int value = 1; };
template<> struct Fib<0> { static constexpr int value = 0; };

[[nodiscard]] constexpr std::array<int, 10> generate_sequence() noexcept {
    std::array<int, 10> seq{};
    for (int i = 0; i < 10; ++i) seq[i] = i * i;
    return seq;
}

template<std::size_t N>
[[nodiscard]] constexpr int sum_array_ct(const std::array<int, N>& a) noexcept {
    int s = 0; for (int v : a) s += v; return s;
}

template<std::size_t N>
[[nodiscard]] constexpr int max_array_ct(const std::array<int, N>& a) noexcept {
    int m = a[0]; for (std::size_t i = 1; i < N; ++i) if (a[i] > m) m = a[i]; return m;
}

template<std::size_t N>
[[nodiscard]] constexpr int min_array_ct(const std::array<int, N>& a) noexcept {
    int m = a[0]; for (std::size_t i = 1; i < N; ++i) if (a[i] < m) m = a[i]; return m;
}

template<std::size_t N>
[[nodiscard]] constexpr int avg_array_ct(const std::array<int, N>& a) noexcept {
    return sum_array_ct(a) / static_cast<int>(N);
}

static_assert(fibonacci_ct(10) == 55);
static_assert(factorial_ct(5)  == 120);
static_assert(factorial_ct(6)  == 720);
static_assert(Fib<10>::value   == 55);
static_assert(power_ct(2, 5)   == 32);
static_assert(power_ct(2, 8)   == 256);
static_assert(is_prime_ct(7));
static_assert(is_prime_ct(29));
static_assert(!is_prime_ct(9));
static_assert(is_even_ct(10));
static_assert(is_odd_ct(3));
static_assert(is_odd_ct(7));
static_assert(abs_ct(-10) == 10);
static_assert(gcd_ct(48, 18) == 6);
static_assert(sum_n_ct(5)  == 15);
static_assert(sum_n_ct(10) == 55);
static_assert(cube_ct(2)   == 8);
static_assert(clamp_ct(120, 0, 100) == 100);
static_assert(clamp_ct(-5,  0, 100) == 0);
static_assert(is_integral_ct_v<int>);
static_assert(!is_integral_ct_v<double>);

constexpr auto SEQ = generate_sequence();
static_assert(max_array_ct(SEQ) == 81);
static_assert(min_array_ct(SEQ) == 0);
static_assert(avg_array_ct(SEQ) == 28);

int main() {
    std::cout << std::boolalpha;

    std::cout << "=== Fibonacci ===\n"
              << "fibonacci_ct(15)=" << fibonacci_ct(15) << "\n"
              << "Fib<10>::value="   << Fib<10>::value   << "\n";

    std::cout << "\n=== Sequence (i^2) ===\n";
    for (int v : SEQ) std::cout << v << " ";
    std::cout << "\n"
              << "sum="  << sum_array_ct(SEQ) << "\n"
              << "max="  << max_array_ct(SEQ) << "\n"
              << "min="  << min_array_ct(SEQ) << "\n"
              << "avg="  << avg_array_ct(SEQ) << "\n";

    std::cout << "\n=== Arithmetic ===\n"
              << "factorial(6)=" << factorial_ct(6)  << "\n"
              << "2^8="          << power_ct(2, 8)   << "\n"
              << "gcd(48,18)="   << gcd_ct(48, 18)   << "\n"
              << "sum_n(10)="    << sum_n_ct(10)      << "\n"
              << "cube(3)="      << cube_ct(3)        << "\n"
              << "abs(-25)="     << abs_ct(-25)       << "\n"
              << "clamp(150,0,100)=" << clamp_ct(150, 0, 100) << "\n";

    std::cout << "\n=== Predicates ===\n"
              << "is_prime(29)="    << is_prime_ct(29)       << "\n"
              << "is_even(42)="     << is_even_ct(42)        << "\n"
              << "is_odd(7)="       << is_odd_ct(7)          << "\n"
              << "is_integral<int>="   << is_integral_ct_v<int>    << "\n"
              << "is_integral<double>=" << is_integral_ct_v<double> << "\n";

    std::cout << "\n=== Runtime algorithms ===\n"
              << "accumulate="      << std::accumulate(SEQ.begin(), SEQ.end(), 0)         << "\n"
              << "max_element="     << *std::ranges::max_element(SEQ)                     << "\n";

    return 0;
}
