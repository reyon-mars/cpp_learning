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

// ----------------------------------
// Main
// ----------------------------------
int main() {

    print_all(1, 2.5, "hello", 3, 4);
    std::cout << "\n";

    std::cout << "Sum: " << sum(1, 2, 3, 4, 5) << "\n";
    std::cout << "Sum with init: " << sum_with_init(10, 20, 30) << "\n";

    std::cout << "Subtract all: " << subtract_all(100, 10, 5) << "\n";

    std::cout << "Average: " << average(2, 4, 6, 8) << "\n";

    print_forward("Forwarded: ", 42, " ", 3.14);

    return 0;
}
