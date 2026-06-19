#include <iostream>
#include <type_traits>   
#include <typeinfo>      
#include <utility>       
#include <cassert>       
#include <vector>        
#include <string>        

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

: proper reference return example
decltype(auto) returnReference() {
    static int x = 50;
    return (x);  // returns reference
}

: helper to print type
template<typename T>
void print_type() {
    std::cout << "Type: " << typeid(T).name() << "\n";
}

// ----------- NEW SMALL ADDITIONS -----------

// Generic add function
template<typename T, typename U>
auto add_generic(T a, U b) {
    return a + b;
}

// Check if type is integral
template<typename T>
void check_integral() {
    std::cout << "Is integral? "
              << std::is_integral<T>::value << "\n";
}

// Universal reference demo
template<typename T>
void universal_reference_demo(T&& value) {
    std::cout << "Universal reference value: "
              << value << "\n";
}

// constexpr deduction helper
constexpr auto square_auto(auto x) {
    return x * x;
}

// Type decay example
template<typename T>
void type_decay_demo(T param) {
    std::cout << "After decay type: "
              << typeid(T).name() << "\n";
}

// Print vector types
template<typename T>
void print_vector_info(const std::vector<T>& vec) {
    std::cout << "Vector size: "
              << vec.size() << "\n";

    std::cout << "Stored type: "
              << typeid(T).name() << "\n";
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

    static_assert(std::is_same<decltype(mul), double>::value,
                  "multiply_generic should return double");

    std::cout << "multiply result: " << mul << "\n";

    : define variable for forwarding test
    int a = 5;

    // ✅ perfect forwarding test
    forward_test(a);   // lvalue
    forward_test(10);  // rvalue

    // ✅ decltype(auto) pitfall
    auto val2 = returnValue();
    std::cout << "returnValue(): " << val2 << "\n";

    : reference behavior demo
    auto ref = returnReference();

    assert(ref == 50);

    ref = 100; // modifies static variable

    std::cout << "Modified reference value: "
              << returnReference() << "\n";

    // ==================================================
    // ✅ EXTRA SMALL FEATURES
    // ==================================================

    std::cout << "\nExtra Type Deduction Features:\n";

    // Generic add
    auto sum = add_generic(10, 2.5);

    std::cout << "Generic add result: "
              << sum << "\n";

    // Integral checks
    check_integral<int>();
    check_integral<double>();

    // Universal reference
    universal_reference_demo(42);

    std::string text = "Hello";
    universal_reference_demo(text);

    // constexpr auto
    constexpr auto sq = square_auto(6);

    static_assert(sq == 36,
                  "square_auto failed");

    std::cout << "square_auto(6): "
              << sq << "\n";

    // Type decay
    const int value = 99;
    type_decay_demo(value);

    // Vector type info
    std::vector<std::string> names = {
        "Alice",
        "Bob",
        "Charlie"
    };

    print_vector_info(names);

    // decltype demo
    decltype(sum) another_sum = 55.5;

    std::cout << "decltype variable: "
              << another_sum << "\n";

    // ==================================================

    std::cout << "Type deduction demonstration completed.\n";
    
    return 0;
}
