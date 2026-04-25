#include <iostream>
#include <type_traits>
#include <cassert>   // ✅ ADDED
#include <utility>   // ✅ ADDED

// -----------------------------------
// Recursive constexpr
// -----------------------------------
constexpr int factorial(int n) {
    return n <= 1 ? 1 : n * factorial(n - 1);
}

constexpr int fibonacci(int n) {
    return n <= 1 ? n : fibonacci(n - 1) + fibonacci(n - 2);
}

// -----------------------------------
// Iterative constexpr (more efficient)
// -----------------------------------
constexpr int factorial_iter(int n) {
    int result = 1;
    for (int i = 2; i <= n; ++i)
        result *= i;
    return result;
}

// -----------------------------------
// consteval (must run at compile-time)
// -----------------------------------
consteval int square(int x) {
    return x * x;
}

// -----------------------------------
// Constexpr class
// -----------------------------------
class ConstexprArray {
private:
    int data[10];

public:
    constexpr ConstexprArray() : data{} {}

    constexpr void set(int index, int value) {
        data[index] = value;
    }

    constexpr int get(int index) const {
        return data[index];
    }

    // -------- SMALL ADDITIONS --------

    constexpr int size() const {
        return 10;
    }

    constexpr int sum() const {
        int s = 0;
        for (int i = 0; i < 10; ++i)
            s += data[i];
        return s;
    }

    // --------------------------------
};

// -----------------------------------
// Template with if constexpr
// -----------------------------------
template<typename T>
constexpr T add(T a, T b) {
    if constexpr (std::is_integral_v<T>)
        return a + b;
    else
        return a + b + static_cast<T>(0.0);
}

// -----------------------------------
// EXTRA CONSTEXPR UTILITIES
// -----------------------------------

// Check even number at compile-time
constexpr bool is_even(int x) {
    return x % 2 == 0;
}

// Power function (constexpr)
constexpr int power(int base, int exp) {
    int result = 1;
    for (int i = 0; i < exp; ++i)
        result *= base;
    return result;
}

// -----------------------------------
// Main
// -----------------------------------
int main() {

    // Compile-time computation
    constexpr int fact5 = factorial(5);
    constexpr int fib10 = fibonacci(10);
    constexpr int fact6_iter = factorial_iter(6);
    constexpr int sq = square(4);

    static_assert(fact5 == 120, "Factorial incorrect!");
    static_assert(fact6_iter == 720, "Iterative factorial incorrect!");
    static_assert(is_even(10), "Even check failed");     // ✅ ADDED
    static_assert(power(3, 3) == 27, "Power failed");    // ✅ ADDED

    std::cout << "5! = " << fact5 << "\n";
    std::cout << "fib(10) = " << fib10 << "\n";
    std::cout << "6! (iter) = " << fact6_iter << "\n";
    std::cout << "square(4) = " << sq << "\n";

    // Runtime computation
    int n = 6;
    std::cout << n << "! = " << factorial(n) << "\n";

    // Constexpr object usage
    constexpr ConstexprArray arr{};
    ConstexprArray runtime_arr;
    runtime_arr.set(0, 42);

    std::cout << "Runtime array[0] = "
              << runtime_arr.get(0) << "\n";

    std::cout << "Runtime array sum = "
              << runtime_arr.sum() << "\n";

    assert(runtime_arr.get(0) == 42);  // ✅ ADDED

    // if constexpr demo
    std::cout << "Add int: " << add(3, 4) << "\n";
    std::cout << "Add double: " << add(1.5, 2.5) << "\n";

    // -------- ADDED USAGE --------

    constexpr int pow_val = power(2, 5);
    static_assert(pow_val == 32, "Power incorrect!");

    std::cout << "2^5 = " << pow_val << "\n";

    std::cout << "Is 10 even? "
              << (is_even(10) ? "Yes\n" : "No\n");

    // ----------------------------

    return 0;
}
