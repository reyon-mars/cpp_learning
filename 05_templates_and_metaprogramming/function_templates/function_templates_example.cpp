#include <iostream>
#include <string>
#include <string_view>
#include <type_traits>
#include <typeinfo>
#include <vector>
#include <array>
#include <span>
#include <cassert>
#include <utility>
#include <algorithm>

template<typename T>
[[nodiscard]] constexpr T max_of(T a, T b) noexcept(noexcept(b < a)) {
    return b < a ? a : b;
}

template<typename T>
[[nodiscard]] int find_position(std::span<const T> arr, const T& value) {
    auto it = std::ranges::find(arr, value);
    return it != arr.end() ? static_cast<int>(it - arr.begin()) : -1;
}

template<typename T>
void print(const T& value) {
    if constexpr (std::is_integral_v<T>)
        std::cout << "Integral: " << value << "\n";
    else
        std::cout << "Value: " << value << "\n";
}

template<>
void print<std::string>(const std::string& value) {
    std::cout << "String: [" << value << "]\n";
}

template<>
void print<bool>(const bool& value) {
    std::cout << "Boolean: " << std::boolalpha << value << "\n";
}

template<>
void print<double>(const double& value) {
    std::cout << "Double: " << value << "\n";
}

template<>
void print<char>(const char& value) {
    std::cout << "Character: '" << value << "'\n";
}

template<typename T, typename U>
void print(const T& a, const U& b) {
    std::cout << "Two values: " << a << ", " << b << "\n";
}

template<typename T, std::size_t N>
void print(const T (&arr)[N]) {
    std::cout << "Array: ";
    for (const auto& v : arr) std::cout << v << " ";
    std::cout << "\n";
}

template<int N>
void print_constant() {
    std::cout << "Constant: " << N << "\n";
}

template<typename T>
struct Printer {
    static void print_value(const T& value) {
        std::cout << "Generic: " << value << "\n";
    }
};

template<typename T>
struct Printer<T*> {
    static void print_value(T* value) {
        std::cout << "Pointer: " << static_cast<void*>(value) << "\n";
    }
};

template<typename T>
struct Printer<const T*> {
    static void print_value(const T* value) {
        std::cout << "Const pointer: " << static_cast<const void*>(value) << "\n";
    }
};

template<typename T>
void print_type(const T&) {
    std::cout << "Type: " << typeid(T).name() << "\n";
}

template<typename T>
void check_reference(T&&) {
    if constexpr (std::is_lvalue_reference_v<T>)
        std::cout << "Lvalue reference\n";
    else
        std::cout << "Rvalue reference\n";
}

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

template<typename... Args>
void print_all(const Args&... args) {
    ((std::cout << args << " "), ...);
    std::cout << "\n";
}

template<typename T = int>
[[nodiscard]] constexpr T multiply(T a, T b) noexcept { return a * b; }

template void print<int>(const int&);

int main() {
    std::cout << "=== print specialisations ===\n";
    print(42);
    print(3.14);
    print(std::string{"Hello"});
    print(true);
    print('A');
    print(10, "test");

    std::cout << "\n=== Non-type template ===\n";
    print_constant<100>();

    std::cout << "\n=== Printer struct ===\n";
    int x = 5;
    Printer<int>::print_value(x);
    Printer<int*>::print_value(&x);
    const int* cptr = &x;
    Printer<const int*>::print_value(cptr);

    std::cout << "\n=== Array print ===\n";
    int arr[5]{1, 2, 3, 4, 5};
    print(arr);

    std::cout << "\n=== print_type / check_reference ===\n";
    print_type(x);
    check_reference(x);
    check_reference(10);

    std::cout << "\n=== max_of ===\n";
    std::cout << "max_of(10,20)="  << max_of(10, 20)    << "\n"
              << "max_of(3.5,7.2)=" << max_of(3.5, 7.2) << "\n";

    std::cout << "\n=== print_all (variadic) ===\n";
    print_all("Templates:", 1, 2.5, 'X');

    std::cout << "\n=== multiply (default type param) ===\n";
    std::cout << "multiply(3,4)="          << multiply(3, 4)            << "\n"
              << "multiply<double>(2.5,4)=" << multiply<double>(2.5, 4.0) << "\n";

    std::cout << "\n=== type_category ===\n";
    type_category<int>();
    type_category<double>();
    type_category<int*>();
    type_category<std::string>();

    std::cout << "\n=== find_position ===\n";
    std::array<int, 5> search{10, 20, 30, 40, 50};
    std::cout << "find 30: pos=" << find_position<int>(search, 30) << "\n"
              << "find 99: pos=" << find_position<int>(search, 99) << "\n";

    std::cout << "\n=== vector ===\n";
    const std::vector<int> vec{1, 2, 3};
    for (int n : vec) std::cout << n << " ";
    std::cout << "\n";

    static_assert(max_of(5, 9) == 9);
    static_assert(multiply(3, 4) == 12);
    assert(find_position<int>(search, 30) == 2);
    assert(find_position<int>(search, 99) == -1);

    return 0;
}
