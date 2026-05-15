// Function Templates Exercise
// Template specialization and instantiation

#include <iostream>
#include <string>
#include <type_traits>
#include <typeinfo>   // ✅ ADDED
#include <vector>     // ✅ NEW ADDED
#include <cassert>    // ✅ NEW ADDED
#include <utility>    // ✅ NEW ADDED

// ----------------------------------
// Generic template
// ----------------------------------
template<typename T>
void print(const T& value) {
    if constexpr (std::is_integral_v<T>)
        std::cout << "Integral: " << value << "\n";
    else
        std::cout << "Value: " << value << "\n";
}

// ----------------------------------
// Specialization for std::string
// ----------------------------------
template<>
void print<std::string>(const std::string& value) {
    std::cout << "String: [" << value << "]\n";
}

// ----------------------------------
// Overload (multiple parameters)
// ----------------------------------
template<typename T, typename U>
void print(const T& a, const U& b) {
    std::cout << "Two values: " << a << ", " << b << "\n";
}

// ----------------------------------
// Non-type template parameter
// ----------------------------------
template<int N>
void print_constant() {
    std::cout << "Constant value: " << N << "\n";
}

// ----------------------------------
// Helper struct for partial specialization
// ----------------------------------
template<typename T>
struct Printer {
    static void print_value(const T& value) {
        std::cout << "Generic struct print: " << value << "\n";
    }
};

template<typename T>
struct Printer<T*> {
    static void print_value(T* value) {
        std::cout << "Pointer address: " << value << "\n";
    }
};

// -------- SMALL ADDITIONS --------

// Overload for arrays
template<typename T, size_t N>
void print(const T (&arr)[N]) {
    std::cout << "Array: ";
    for (size_t i = 0; i < N; ++i)
        std::cout << arr[i] << " ";
    std::cout << "\n";
}

// Special handling for bool
template<>
void print<bool>(const bool& value) {
    std::cout << "Boolean: " << (value ? "true" : "false") << "\n";
}

// Printer specialization for const pointers
template<typename T>
struct Printer<const T*> {
    static void print_value(const T* value) {
        std::cout << "Const pointer address: " << value << "\n";
    }
};

// -------- NEW SMALL ADDITIONS --------

// Floating-point specialization
template<>
void print<double>(const double& value) {
    std::cout << "Double (specialized): " << value << "\n";
}

// Print type information
template<typename T>
void print_type(const T&) {
    std::cout << "Type: " << typeid(T).name() << "\n";
}

// Detect reference type
template<typename T>
void check_reference(T&& value) {
    if constexpr (std::is_lvalue_reference_v<T>)
        std::cout << "Lvalue reference detected\n";
    else
        std::cout << "Rvalue reference detected\n";
}

// ---------------------------------
// EXTRA SMALL ADDITIONS
// ---------------------------------

// Template returning max value
template<typename T>
T max_value(const T& a, const T& b) {
    return (a > b) ? a : b;
}

// Variadic template print
template<typename... Args>
void print_all(const Args&... args) {
    ((std::cout << args << " "), ...);
    std::cout << "\n";
}

// Template specialization for char
template<>
void print<char>(const char& value) {
    std::cout << "Character: '" << value << "'\n";
}

// Template with default type
template<typename T = int>
T multiply(T a, T b) {
    return a * b;
}

// Check type category
template<typename T>
void type_category() {
    if constexpr (std::is_pointer_v<T>)
        std::cout << "Pointer type\n";
    else if constexpr (std::is_floating_point_v<T>)
        std::cout << "Floating-point type\n";
    else if constexpr (std::is_integral_v<T>)
        std::cout << "Integral type\n";
    else
        std::cout << "Other type\n";
}

// ---------------------------------

// Explicit instantiation
template void print<int>(const int&);

int main() {

    print(42);
    print(3.14);
    print(std::string("Hello"));
    print(10, "test");

    print_constant<100>();

    int x = 5;
    Printer<int>::print_value(x);
    Printer<int*>::print_value(&x);

    // -------- ADDED USAGE --------

    bool flag = true;
    print(flag);

    int arr[5] = {1, 2, 3, 4, 5};
    print(arr);

    const int* cptr = &x;
    Printer<const int*>::print_value(cptr);

    // -------- NEW USAGE --------

    print_type(x);

    check_reference(x);     // lvalue
    check_reference(10);    // rvalue

    // -------- EXTRA NEW USAGE --------

    print('A');

    std::cout << "Max value: "
              << max_value(10, 20) << "\n";

    std::cout << "Max double: "
              << max_value(3.5, 7.2) << "\n";

    print_all("Templates:", 1, 2.5, 'X');

    std::cout << "Multiply int: "
              << multiply(3, 4) << "\n";

    std::cout << "Multiply double: "
              << multiply<double>(2.5, 4.0) << "\n";

    type_category<int>();
    type_category<double>();
    type_category<int*>();

    // Vector demo with templates
    std::vector<int> vec = {1, 2, 3};

    std::cout << "Vector contents: ";
    for (const auto& n : vec)
        std::cout << n << " ";
    std::cout << "\n";

    // Small validation
    assert(max_value(5, 9) == 9);

    // ----------------------------

    return 0;
}
