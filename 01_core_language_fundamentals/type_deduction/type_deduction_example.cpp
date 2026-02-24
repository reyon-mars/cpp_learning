// Type Deduction Exercise
// auto, decltype, and deduction rules

#include <iostream>
#include <type_traits>
#include <typeinfo>

// helper to print type name
template<typename T>
void print_type() {
    std::cout << "Type: " << typeid(T).name() << "\n";
}

int main() {
    auto x = 10;           // int
    auto y = 3.14;         // double

    int arr[] = {1, 2, 3};
    auto z = arr;          // int*

    int a = 5;
    decltype(a) b = 10;    // int

    // -------- SMALL ADDITIONS --------

    print_type<decltype(x)>();
    print_type<decltype(y)>();
    print_type<decltype(z)>();
    print_type<decltype(b)>();

    std::cout << std::boolalpha;
    std::cout << "x is int? "
              << std::is_same<decltype(x), int>::value << "\n";
    std::cout << "y is double? "
              << std::is_same<decltype(y), double>::value << "\n";

    decltype((a)) c = a;   // int& (reference)
    std::cout << "decltype((a)) is reference? "
              << std::is_reference<decltype(c)>::value << "\n";

    // --------------------------------

    return 0;
}
