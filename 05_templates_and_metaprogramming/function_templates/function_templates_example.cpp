// Function Templates Exercise
// Template specialization and instantiation

#include <iostream>
#include <string>
#include <type_traits>

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
// (Function templates cannot be partially specialized)
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

    return 0;
}
