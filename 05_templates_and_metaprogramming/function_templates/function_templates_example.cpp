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

    // ----------------------------

    return 0;
}
