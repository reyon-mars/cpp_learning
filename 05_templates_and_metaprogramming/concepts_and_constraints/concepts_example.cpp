// Concepts and Constraints Exercise (C++20)
// Compile-time constraints on template parameters

#include <iostream>
#include <concepts>
#include <vector>
#include <list>
#include <type_traits>

// -----------------------------
// Numeric Concept
// -----------------------------
template<typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;

template<Numeric T>
T add(T a, T b) {
    return a + b;
}

// Alternative requires syntax
template<typename T>
requires Numeric<T>
T multiply(T a, T b) {
    return a * b;
}

// -----------------------------
// Container Concept
// -----------------------------
template<typename T>
concept Container = requires(T t) {
    { t.begin() };
    { t.end() };
    { t.size() } -> std::convertible_to<std::size_t>;
};

// -----------------------------
// Printable Concept
// -----------------------------
template<typename T>
concept Printable = requires(T t) {
    { std::cout << t } -> std::same_as<std::ostream&>;
};

template<Container C>
void print_container(const C& cont) {
    std::cout << "Size: " << cont.size() << "\n";
}

// -----------------------------
// Constrained Class Template
// -----------------------------
template<Numeric T>
class Calculator {
public:
    T square(T value) const {
        return value * value;
    }

    // -------- SMALL ADDITIONS --------

    T cube(T value) const {
        return value * value * value;
    }

    bool is_positive(T value) const {
        return value > 0;
    }

    // --------------------------------
};

// -----------------------------
// EXTRA UTILITIES (SMALL)
// -----------------------------

// Print only if Printable
template<Printable T>
void print_value(const T& value) {
    std::cout << "Printable value: " << value << "\n";
}

// Sum all elements of a container
template<Container C>
auto sum_container(const C& cont) {
    using T = typename C::value_type;
    T sum{};
    for (const auto& v : cont)
        sum += v;
    return sum;
}

// -----------------------------
// Main
// -----------------------------
int main() {

    std::cout << "Sum (int): " << add(5, 3) << "\n";
    std::cout << "Sum (double): " << add(1.5, 2.5) << "\n";

    std::cout << "Multiply (int): " << multiply(4, 6) << "\n";

    std::vector<int> vec = {1, 2, 3};
    print_container(vec);

    std::list<double> lst = {1.1, 2.2, 3.3};
    print_container(lst);

    Calculator<int> calc;
    std::cout << "Square: " << calc.square(7) << "\n";

    // -------- ADDED USAGE --------

    std::cout << "Cube: " << calc.cube(3) << "\n";
    std::cout << "Is positive? "
              << (calc.is_positive(7) ? "Yes\n" : "No\n");

    print_value(123);
    print_value(3.14);

    std::cout << "Sum of vector: "
              << sum_container(vec) << "\n";

    std::cout << "Sum of list: "
              << sum_container(lst) << "\n";

    // ----------------------------

    return 0;
}
