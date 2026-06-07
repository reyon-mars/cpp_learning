#include <iostream>
#include <type_traits>
#include <cassert>
#include <utility>
#include <array>
#include <string_view>
#include <numeric>
#include <algorithm>

[[nodiscard]] constexpr int factorial(int n) noexcept {
    return n <= 1 ? 1 : n * factorial(n - 1);
}

[[nodiscard]] constexpr int fibonacci(int n) noexcept {
    return n <= 1 ? n : fibonacci(n - 1) + fibonacci(n - 2);
}

[[nodiscard]] constexpr int factorial_iter(int n) noexcept {
    int result = 1;
    for (int i = 2; i <= n; ++i) result *= i;
    return result;
}

[[nodiscard]] consteval int square(int x) noexcept {
    return x * x;
}

[[nodiscard]] constexpr bool is_even(int x)  noexcept { return x % 2 == 0; }
[[nodiscard]] constexpr int  max_value(int a, int b) noexcept { return a > b ? a : b; }

[[nodiscard]] constexpr int power(int base, int exp) noexcept {
    int result = 1;
    for (int i = 0; i < exp; ++i) result *= base;
    return result;
}

[[nodiscard]] constexpr bool is_prime(int n) noexcept {
    if (n <= 1) return false;
    for (int i = 2; i * i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
}

[[nodiscard]] constexpr std::size_t str_length(std::string_view str) noexcept {
    return str.size();
}

template<std::size_t N>
[[nodiscard]] constexpr int array_sum(const std::array<int, N>& arr) noexcept {
    int sum = 0;
    for (int v : arr) sum += v;
    return sum;
}

constexpr void constexpr_swap(int& a, int& b) noexcept {
    int tmp = a; a = b; b = tmp;
}

class ConstexprArray {
public:
    constexpr ConstexprArray() = default;

    constexpr void set(int index, int value) noexcept { data_[index] = value; }
    constexpr void fill(int value) noexcept { data_.fill(value); }

    [[nodiscard]] constexpr int  get(int index)  const noexcept { return data_[index]; }
    [[nodiscard]] constexpr int  size()          const noexcept { return static_cast<int>(data_.size()); }
    [[nodiscard]] constexpr int  sum()           const noexcept {
        int s = 0;
        for (int v : data_) s += v;
        return s;
    }

private:
    std::array<int, 10> data_{};
};

template<typename T>
[[nodiscard]] constexpr T add(T a, T b) noexcept {
    if constexpr (std::is_integral_v<T>)
        return a + b;
    else
        return a + b + T{};
}

int main() {
    static_assert(factorial(5)      == 120);
    static_assert(factorial_iter(6) == 720);
    static_assert(fibonacci(10)     == 55);
    static_assert(square(4)         == 16);
    static_assert(is_even(10));
    static_assert(!is_even(7));
    static_assert(power(3, 3)       == 27);
    static_assert(power(2, 5)       == 32);
    static_assert(max_value(10, 20) == 20);
    static_assert(is_prime(13));
    static_assert(is_prime(17));
    static_assert(!is_prime(1));
    static_assert(!is_prime(9));
    static_assert(str_length("constexpr") == 9);

    constexpr std::array<int, 5> nums{1, 2, 3, 4, 5};
    static_assert(array_sum(nums) == 15);

    constexpr int fact5      = factorial(5);
    constexpr int fib10      = fibonacci(10);
    constexpr int fact6_iter = factorial_iter(6);
    constexpr int sq         = square(4);
    constexpr int pow_val    = power(2, 5);

    std::cout << "5!="           << fact5      << "\n"
              << "fib(10)="      << fib10      << "\n"
              << "6! (iter)="    << fact6_iter << "\n"
              << "square(4)="    << sq         << "\n"
              << "2^5="          << pow_val    << "\n";

    std::cout << "\n--- Runtime constexpr ===\n";
    int n = 6;
    std::cout << n << "!=" << factorial(n) << "\n";

    std::cout << "\n=== ConstexprArray ===\n";
    ConstexprArray arr;
    arr.set(0, 42);
    std::cout << "arr[0]=" << arr.get(0) << " sum=" << arr.sum() << "\n";
    assert(arr.get(0) == 42);

    arr.fill(3);
    std::cout << "After fill(3) sum=" << arr.sum() << "\n";

    std::cout << "\n=== if constexpr ===\n";
    std::cout << "add(int)=    " << add(3, 4)     << "\n"
              << "add(double)= " << add(1.5, 2.5) << "\n";

    std::cout << "\n=== Predicates ===\n";
    std::cout << "is_even(10)=" << std::boolalpha << is_even(10) << "\n"
              << "max(7,15)="   << max_value(7, 15)               << "\n"
              << "is_prime(17)=" << is_prime(17)                  << "\n"
              << "str_length(\"CompileTime\")=" << str_length("CompileTime") << "\n"
              << "array_sum(nums)=" << array_sum(nums)            << "\n";

    std::cout << "\n=== constexpr_swap ===\n";
    int a = 5, b = 10;
    constexpr_swap(a, b);
    std::cout << "After swap: a=" << a << " b=" << b << "\n";

    return 0;
}
