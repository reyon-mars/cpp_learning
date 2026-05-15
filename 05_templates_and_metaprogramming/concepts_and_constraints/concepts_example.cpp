#include <iostream>
#include <concepts>
#include <vector>
#include <list>
#include <type_traits>
#include <cassert>   // ✅ ADDED
#include <utility>   // ✅ ADDED
#include <array>     // ✅ ADDED
#include <string>    // ✅ ADDED
#include <algorithm> // ✅ ADDED

// -----------------------------
// Numeric Concept
// -----------------------------
template<typename T>
concept Numeric =
    std::integral<T> ||
    std::floating_point<T>;

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

// ✅ ADDED: subtraction
template<Numeric T>
T subtract(T a, T b) {
    return a - b;
}

// ✅ ADDED: division
template<Numeric T>
T divide(T a, T b) {
    return a / b;
}

// -----------------------------
// Container Concept
// -----------------------------
template<typename T>
concept Container = requires(T t) {
    { t.begin() };
    { t.end() };
    { t.size() }
        -> std::convertible_to<std::size_t>;
};

// -----------------------------
// Printable Concept
// -----------------------------
template<typename T>
concept Printable = requires(T t) {
    { std::cout << t }
        -> std::same_as<std::ostream&>;
};

template<Container C>
void print_container(const C& cont) {
    std::cout << "Size: "
              << cont.size() << "\n";
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

    // ✅ ADDED: absolute value
    T absolute(T value) const {
        return value < 0 ? -value : value;
    }

    // --------------------------------
};

// -----------------------------
// EXTRA UTILITIES (SMALL)
// -----------------------------

// Print only if Printable
template<Printable T>
void print_value(const T& value) {
    std::cout << "Printable value: "
              << value << "\n";
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

// ✅ ADDED: average of container
template<Container C>
auto average_container(const C& cont) {
    return static_cast<double>(
               sum_container(cont))
           / cont.size();
}

// ✅ ADDED: print elements
template<Container C>
void print_elements(const C& cont) {
    for (const auto& v : cont)
        std::cout << v << " ";

    std::cout << "\n";
}

// ✅ ADDED: comparable concept
template<typename T>
concept Comparable = requires(T a, T b) {
    { a == b } -> std::convertible_to<bool>;
};

// ✅ ADDED: equality check
template<Comparable T>
bool are_equal(const T& a, const T& b) {
    return a == b;
}

// ✅ ADDED: increment helper
template<Numeric T>
void increment(T& value) {
    ++value;
}

// -----------------------------
// Main
// -----------------------------
int main() {

    std::cout << "Sum (int): "
              << add(5, 3) << "\n";

    std::cout << "Sum (double): "
              << add(1.5, 2.5) << "\n";

    std::cout << "Multiply (int): "
              << multiply(4, 6) << "\n";

    std::vector<int> vec = {1, 2, 3};

    print_container(vec);

    std::list<double> lst = {
        1.1, 2.2, 3.3
    };

    print_container(lst);

    Calculator<int> calc;

    std::cout << "Square: "
              << calc.square(7) << "\n";

    // -------- ADDED USAGE --------

    std::cout << "Cube: "
              << calc.cube(3) << "\n";

    std::cout << "Is positive? "
              << (calc.is_positive(7)
                      ? "Yes\n"
                      : "No\n");

    print_value(123);
    print_value(3.14);

    std::cout << "Sum of vector: "
              << sum_container(vec)
              << "\n";

    std::cout << "Sum of list: "
              << sum_container(lst)
              << "\n";

    // ✅ ADDED: compile-time validation
    static_assert(Numeric<int>);
    static_assert(Numeric<double>);

    // ✅ ADDED: runtime validation
    assert(sum_container(vec) == 6);

    // -------- NEW FEATURE USAGE --------

    std::cout << "\n--- Arithmetic ---\n";

    std::cout << "Subtract: "
              << subtract(10, 3)
              << "\n";

    std::cout << "Divide: "
              << divide(20.0, 4.0)
              << "\n";

    std::cout << "\n--- Absolute Value ---\n";

    std::cout << "Absolute(-9): "
              << calc.absolute(-9)
              << "\n";

    std::cout << "\n--- Average ---\n";

    std::cout << "Average vector: "
              << average_container(vec)
              << "\n";

    std::cout << "\n--- Print Elements ---\n";

    print_elements(vec);

    std::array<int, 4> arr = {
        4, 5, 6, 7
    };

    print_container(arr);

    std::cout << "Array sum: "
              << sum_container(arr)
              << "\n";

    std::cout << "\n--- Comparable Demo ---\n";

    std::cout << "10 == 10 ? "
              << (are_equal(10, 10)
                      ? "Yes\n"
                      : "No\n");

    std::cout << "Hello == World ? "
              << (are_equal(
                      std::string("Hello"),
                      std::string("World"))
                      ? "Yes\n"
                      : "No\n");

    std::cout << "\n--- Increment Demo ---\n";

    int value = 5;

    increment(value);

    std::cout << "Incremented value: "
              << value << "\n";

    std::cout << "\n--- Lambda + Concepts ---\n";

    auto printer = []<Printable T>(
                       const T& value) {
        std::cout << "Lambda print: "
                  << value << "\n";
    };

    printer(999);
    printer(std::string("Concepts"));

    std::cout << "\n--- Sorting Vector ---\n";

    std::sort(vec.begin(), vec.end(),
        [](int a, int b) {
            return a > b;
        });

    print_elements(vec);

    // ----------------------------

    return 0;
}
