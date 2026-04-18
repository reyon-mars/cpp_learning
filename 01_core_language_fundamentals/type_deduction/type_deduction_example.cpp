#include <iostream>
#include <type_traits>   // ✅ ADDED
#include <typeinfo>      // ✅ ADDED

// ----------- MORE ADVANCED ADDITIONS -----------

// Trailing return type (RENAMED to avoid conflict)
template<typename T, typename U>
auto multiply_generic(T a, U b) -> decltype(a * b) {
    return a * b;
}

// Perfect forwarding example
template<typename T>
void forward_test(T&& param) {
    if constexpr (std::is_lvalue_reference<T>::value) {
        std::cout << "Lvalue passed\n";
    } else {
        std::cout << "Rvalue passed\n";
    }
}

// decltype(auto) pitfall example
decltype(auto) returnValue() {
    int x = 10;
    return x;  // returns by value
}

// ✅ ADDED: proper reference return example
decltype(auto) returnReference() {
    static int x = 50;
    return (x);  // returns reference
}

// ✅ ADDED: helper to print type
template<typename T>
void print_type() {
    std::cout << "Type: " << typeid(T).name() << "\n";
}

// ----------------------------------------------

int main() {

    std::cout << "\nAdvanced Deduction Concepts:\n";

    // ✅ initializer list behavior
    auto list = {1, 2, 3}; // std::initializer_list<int>
    print_type<decltype(list)>();

    // ✅ top-level vs low-level const
    const int ci = 10;
    auto copy = ci;          // int
    const auto copy2 = ci;   // const int

    std::cout << "copy is const? "
              << std::is_const<decltype(copy)>::value << "\n";

    std::cout << "copy2 is const? "
              << std::is_const<decltype(copy2)>::value << "\n";

    // ✅ trailing return type
    auto mul = multiply_generic(2, 3.5);
    std::cout << "multiply result: " << mul << "\n";

    // ✅ ADDED: define variable for forwarding test
    int a = 5;

    // ✅ perfect forwarding test
    forward_test(a);   // lvalue
    forward_test(10);  // rvalue

    // ✅ decltype(auto) pitfall
    auto val2 = returnValue();
    std::cout << "returnValue(): " << val2 << "\n";

    // ✅ ADDED: reference behavior demo
    auto ref = returnReference();
    ref = 100; // modifies static variable

    std::cout << "Modified reference value: " << returnReference() << "\n";

    return 0;
}
