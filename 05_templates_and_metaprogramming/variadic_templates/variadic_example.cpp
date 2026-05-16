// Variadic Templates Exercise
// Parameter packs and fold expressions

#include <iostream>
#include <utility>
#include <type_traits>
#include <algorithm>   // 🔹 ADDED
#include <tuple>       // 🔹 ADDED
#include <cassert>     // 🔹 ADDED

// ----------------------------------
// Recursive case (base)
// ----------------------------------
template<typename T>
void print_all(const T& first) {
    std::cout << first << "\n";
}

// ----------------------------------
// Variadic recursive case
// ----------------------------------
template<typename T, typename... Args>
void print_all(const T& first,
               const Args&... rest) {

    std::cout << first << " ";
    print_all(rest...);
}

// ----------------------------------
// Fold expression (left fold)
// ----------------------------------
template<typename... Args>
auto sum(Args... args) {

    static_assert(
        (std::is_arithmetic_v<Args> && ...),
        "All arguments must be arithmetic"
    );

    return (args + ...);
}

// ----------------------------------
// Fold with initial value
// ----------------------------------
template<typename... Args>
auto sum_with_init(Args... args) {
    return (0 + ... + args);
}

// ----------------------------------
// Right fold example
// ----------------------------------
template<typename... Args>
auto subtract_all(Args... args) {
    return (args - ...);
}

// ----------------------------------
// Variadic average
// ----------------------------------
template<typename... Args>
auto average(Args... args) {

    static_assert(sizeof...(args) > 0,
                  "At least one argument required");

    return sum(args...) / sizeof...(args);
}

// ----------------------------------
// Perfect forwarding print
// ----------------------------------
template<typename... Args>
void print_forward(Args&&... args) {

    (std::cout << ... << args) << "\n";
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

    static_assert(
        (std::is_arithmetic_v<Args> && ...),
        "All arguments must be arithmetic"
    );

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

// Conditional sum
template<typename Pred, typename... Args>
auto sum_if(Pred p, Args... args) {

    return ((p(args) ? args : 0) + ...);
}

// ======================================================
// EXTRA SMALL ADDITIONS
// ======================================================

// Check if any argument is floating point
template<typename... Args>
constexpr bool any_floating() {

    return (std::is_floating_point_v<Args> || ...);
}

// Print argument types count
template<typename... Args>
void print_pack_info() {

    std::cout << "Pack contains "
              << sizeof...(Args)
              << " arguments\n";
}

// Convert parameter pack to tuple
template<typename... Args>
auto make_tuple_pack(Args&&... args) {

    return std::make_tuple(
        std::forward<Args>(args)...);
}

// Fold expression for comma-separated output
template<typename... Args>
void print_csv(Args&&... args) {

    std::size_t n = 0;

    ((std::cout << args
                << (++n < sizeof...(Args) ? ", " : "")), ...);

    std::cout << "\n";
}

// Count how many arguments satisfy predicate
template<typename Pred, typename... Args>
std::size_t count_if_pack(Pred p, Args... args) {

    return ((p(args) ? 1 : 0) + ...);
}

// ======================================================
// Main
// ======================================================
int main() {

    print_all(1, 2.5, "hello", 3, 4);
    std::cout << "\n";

    std::cout << "Sum: "
              << sum(1, 2, 3, 4, 5)
              << "\n";

    std::cout << "Sum with init: "
              << sum_with_init(10, 20, 30)
              << "\n";

    std::cout << "Subtract all: "
              << subtract_all(100, 10, 5)
              << "\n";

    std::cout << "Average: "
              << average(2, 4, 6, 8)
              << "\n";

    print_forward("Forwarded: ", 42, " ", 3.14);

    // -------- Added usage --------
    std::cout << "\n--- Extra Tests ---\n";

    std::cout << "Count args: "
              << count_args(1, 2, 3, 4, 5)
              << "\n";

    std::cout << "All same (int,int,int): "
              << all_same<int, int, int, int>()
              << "\n";

    std::cout << "All same (int,double): "
              << all_same<int, double>()
              << "\n";

    std::cout << "Multiply: "
              << multiply(2, 3, 4)
              << "\n";

    std::cout << "\nPrint lines:\n";
    print_lines("Line 1", 123, 4.56);

    // -------- NEW USAGE --------

    std::cout << "\n--- More Variadic Features ---\n";

    std::cout << "Min: "
              << min_value(5, 2, 8, 1)
              << "\n";

    std::cout << "Max: "
              << max_value(5, 2, 8, 1)
              << "\n";

    std::cout << "Logical AND: "
              << logical_and(true, true, false)
              << "\n";

    std::cout << "Logical OR: "
              << logical_or(false, false, true)
              << "\n";

    std::cout << "\nInvoke all:\n";

    invoke_all([](auto x) {
        std::cout << "Value: "
                  << x << "\n";
    }, 1, 2, 3);

    std::cout << "Sum if even: "
              << sum_if([](int x) {
                    return x % 2 == 0;
                },
                1, 2, 3, 4, 5, 6)
              << "\n";

    // ======================================================
    // EXTRA SMALL USAGE
    // ======================================================

    std::cout << "\n--- Advanced Pack Utilities ---\n";

    std::cout << "Any floating? "
              << any_floating<int, char, double>()
              << "\n";

    print_pack_info<int, double, char>();

    auto tup = make_tuple_pack(1, 2.5, "tuple");
    std::cout << "Tuple created with "
              << std::tuple_size_v<decltype(tup)>
              << " elements\n";

    std::cout << "CSV output: ";
    print_csv(10, 20, 30, 40);

    std::cout << "Count even numbers: "
              << count_if_pack(
                     [](int x) {
                         return x % 2 == 0;
                     },
                     1, 2, 3, 4, 5, 6)
              << "\n";

    // ✅ Compile-time validation
    static_assert(any_floating<int, double>());
    static_assert(!any_floating<int, char>());

    // ✅ Runtime validation
    assert(sum(1, 2, 3) == 6);

    // ======================================================

    return 0;
}
