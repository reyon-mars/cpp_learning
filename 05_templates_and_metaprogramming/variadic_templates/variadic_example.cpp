// Variadic Templates Exercise
// Parameter packs and fold expressions

#include <iostream>
#include <utility>
#include <type_traits>

// ----------------------------------
// Recursive case (base)
template<typename T>
void print_all(const T& first) {
    std::cout << first << "\n";
}

// ----------------------------------
// Variadic recursive case
template<typename T, typename... Args>
void print_all(const T& first, const Args&... rest) {
    std::cout << first << " ";
    print_all(rest...);
}

// ----------------------------------
// Fold expression (left fold)
template<typename... Args>
auto sum(Args... args) {
    static_assert((std::is_arithmetic_v<Args> && ...),
                  "All arguments must be arithmetic");
    return (args + ...);  // Left fold
}

// ----------------------------------
// Fold with initial value
template<typename... Args>
auto sum_with_init(Args... args) {
    return (0 + ... + args);  // Explicit initial value
}

// ----------------------------------
// Right fold example
template<typename... Args>
auto subtract_all(Args... args) {
    return (args - ...);  // Right fold
}

// ----------------------------------
// Variadic average
template<typename... Args>
auto average(Args... args) {
    static_assert(sizeof...(args) > 0, "At least one argument required");
    return sum(args...) / sizeof...(args);
}

// ----------------------------------
// Perfect forwarding print
template<typename... Args>
void print_forward(Args&&... args) {
    (std::cout << ... << args) << "\n";  // Fold over <<
}

// ---------------- SMALL ADDITIONS ----------------

// Count number of arguments
template<typename... Args>
constexpr std::size_t count_args(Args...) {
    return sizeof...(Args);
}

// Check if all arguments are same type
template<typename T, typename... Args>
constexpr bool all_same() {
    return (std::is_same_v<T, Args> && ...);
}

// Multiply using fold expression
template<typename... Args>
auto multiply(Args... args) {
    static_assert((std::is_arithmetic_v<Args> && ...),
                  "All arguments must be arithmetic");
    return (args * ...);
}

// Print each argument on new line using fold
template<typename... Args>
void print_lines(Args&&... args) {
    ((std::cout << args << "\n"), ...);
}

// ----------- NEW ADDITIONS -----------

// Minimum value using fold
template<typename... Args>
auto min_value(Args... args) {
    return (std::min)({args...});
}

// Maximum value using fold
template<typename... Args>
auto max_value(Args... args) {
    return (std::max)({args...});
}

// Logical AND fold
template<typename... Args>
bool logical_and(Args... args) {
    return (args && ...);
}

// Logical OR fold
template<typename... Args>
bool logical_or(Args... args) {
    return (args || ...);
}

// Apply a function to all arguments
template<typename Func, typename... Args>
void invoke_all(Func f, Args&&... args) {
    (f(std::forward<Args>(args)), ...);
}

// Conditional sum (only values passing predicate)
template<typename Pred, typename... Args>
auto sum_if(Pred p, Args... args) {
    return ( (p(args) ? args : 0) + ... );
}

// ------------------------------------------------
// Main
// ------------------------------------------------
int main() {

    print_all(1, 2.5, "hello", 3, 4);
    std::cout << "\n";

    std::cout << "Sum: " << sum(1, 2, 3, 4, 5) << "\n";
    std::cout << "Sum with init: " << sum_with_init(10, 20, 30) << "\n";

    std::cout << "Subtract all: " << subtract_all(100, 10, 5) << "\n";

    std::cout << "Average: " << average(2, 4, 6, 8) << "\n";

    print_forward("Forwarded: ", 42, " ", 3.14);

    // -------- Added usage --------
    std::cout << "\n--- Extra Tests ---\n";

    std::cout << "Count args: "
              << count_args(1, 2, 3, 4, 5) << "\n";

    std::cout << "All same (int,int,int): "
              << all_same<int, int, int, int>() << "\n";

    std::cout << "All same (int,double): "
              << all_same<int, double>() << "\n";

    std::cout << "Multiply: "
              << multiply(2, 3, 4) << "\n";

    std::cout << "\nPrint lines:\n";
    print_lines("Line 1", 123, 4.56);

    // -------- NEW USAGE --------

    std::cout << "\n--- More Variadic Features ---\n";

    std::cout << "Min: " << min_value(5, 2, 8, 1) << "\n";
    std::cout << "Max: " << max_value(5, 2, 8, 1) << "\n";

    std::cout << "Logical AND: "
              << logical_and(true, true, false) << "\n";

    std::cout << "Logical OR: "
              << logical_or(false, false, true) << "\n";

    std::cout << "\nInvoke all:\n";
    invoke_all([](auto x) {
        std::cout << "Value: " << x << "\n";
    }, 1, 2, 3);

    std::cout << "Sum if even: "
              << sum_if([](int x){ return x % 2 == 0; },
                        1, 2, 3, 4, 5, 6)
              << "\n";

    // ----------------------------

    return 0;
}
