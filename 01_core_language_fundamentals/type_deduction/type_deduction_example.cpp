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

// ----------- NEW ADDITIONS -----------

// Function using auto return type
auto add(int a, int b) {
    return a + b;
}

// Function using decltype(auto)
decltype(auto) getRef(int& x) {
    return (x); // returns reference
}

// ------------------------------------

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


    // -------- NEW FEATURE USAGE --------

    const int cx = 20;
    auto d = cx;          // int (const removed)
    auto& e = cx;         // const int&

    print_type<decltype(d)>();
    print_type<decltype(e)>();

    std::cout << "d is const? "
              << std::is_const<decltype(d)>::value << "\n";

    std::cout << "e is reference? "
              << std::is_reference<decltype(e)>::value << "\n";

    // auto&& (universal reference)
    auto&& f = a;
    std::cout << "f is reference? "
              << std::is_reference<decltype(f)>::value << "\n";

    // decltype(auto)
    int val = 50;
    decltype(auto) refVal = getRef(val);
    refVal = 100; // modifies original

    std::cout << "val after refVal change: "
              << val << "\n";

    // auto return type
    auto result = add(3, 4);
    std::cout << "add result: " << result << "\n";

    // ----------------------------------

    return 0;
}
