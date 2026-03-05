// Type Traits Workshop
// Compile-time type introspection

#include <iostream>
#include <type_traits>
#include <string>

// ----------------------------------
// Custom type trait example
// ----------------------------------
template<typename T>
struct is_string : std::false_type {};

template<>
struct is_string<std::string> : std::true_type {};

// ----------------------------------
// Type analyzer
// ----------------------------------
template<typename T>
void analyze_type() {
    std::cout << "Type analysis:\n";
    std::cout << "  is_integral: " << std::is_integral_v<T> << "\n";
    std::cout << "  is_pointer: " << std::is_pointer_v<T> << "\n";
    std::cout << "  is_class: " << std::is_class_v<T> << "\n";
    std::cout << "  is_trivial: " << std::is_trivial_v<T> << "\n";
    std::cout << "  is_const: " << std::is_const_v<T> << "\n";
    std::cout << "  is_reference: " << std::is_reference_v<T> << "\n";
    std::cout << "  is_arithmetic: " << std::is_arithmetic_v<T> << "\n";
    std::cout << "  is_string (custom): " << is_string<T>::value << "\n";
    std::cout << "  size: " << sizeof(T) << "\n";

    // Compile-time branching
    if constexpr (std::is_pointer_v<T>) {
        std::cout << "  -> This is a pointer type\n";
    }
    else if constexpr (std::is_integral_v<T>) {
        std::cout << "  -> This is an integral type\n";
    }
    else if constexpr (std::is_class_v<T>) {
        std::cout << "  -> This is a class type\n";
    }
}

// ----------------------------------
// Main
// ----------------------------------
int main() {

    static_assert(std::is_same_v<int, int>);
    static_assert(!std::is_same_v<int, double>);

    std::cout << "Analyzing int:\n";
    analyze_type<int>();

    std::cout << "\nAnalyzing int*:\n";
    analyze_type<int*>();

    std::cout << "\nAnalyzing const double&:\n";
    analyze_type<const double&>();

    std::cout << "\nAnalyzing std::string:\n";
    analyze_type<std::string>();

    return 0;
}
