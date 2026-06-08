#include <iostream>
#include <vector>
#include <type_traits>
#include <string>
#include <string_view>
#include <array>
#include <cassert>
#include <utility>
#include <concepts>

template<typename T>
struct traits {
    static constexpr std::string_view name = "Generic";
    static void print() { std::cout << "Generic template\n"; }
};

template<>
struct traits<int> {
    static constexpr std::string_view name = "int";
    static void print() { std::cout << "Specialized for int\n"; }
};

template<typename T>
struct traits<T*> {
    static constexpr std::string_view name = "pointer";
    static void print() { std::cout << "Specialized for pointer\n"; }
};

template<typename T>
struct traits<const T*> {
    static constexpr std::string_view name = "const pointer";
    static void print() { std::cout << "Specialized for const pointer\n"; }
};

template<typename T>
struct traits<const T> {
    static constexpr std::string_view name = "const";
    static void print() { std::cout << "Specialized for const\n"; }
};

template<typename T>
struct traits<T&> {
    static constexpr std::string_view name = "lvalue reference";
    static void print() { std::cout << "Specialized for lvalue reference\n"; }
};

template<typename T>
struct traits<T&&> {
    static constexpr std::string_view name = "rvalue reference";
    static void print() { std::cout << "Specialized for rvalue reference\n"; }
};

template<typename T>
struct traits<std::vector<T>> {
    static constexpr std::string_view name = "vector<T>";
    static void print() { std::cout << "Specialized for vector<T>\n"; }
};

template<>
struct traits<std::vector<int>> {
    static constexpr std::string_view name = "vector<int>";
    static void print() { std::cout << "Specialized for vector<int>\n"; }
};

template<typename T, std::size_t N>
struct traits<T[N]> {
    static constexpr std::string_view name = "C-array";
    static void print() { std::cout << "Specialized for C-array\n"; }
};

template<typename T, std::size_t N>
struct traits<std::array<T, N>> {
    static constexpr std::string_view name = "std::array";
    static void print() { std::cout << "Specialized for std::array\n"; }
};

template<typename R, typename... Args>
struct traits<R(Args...)> {
    static constexpr std::string_view name = "function type";
    static void print() { std::cout << "Specialized for function type\n"; }
};

template<typename T>
concept HasValueType = requires { typename T::value_type; };

template<typename T>
void print_value_type_info() {
    if constexpr (HasValueType<T>)
        std::cout << "Has value_type\n";
    else
        std::cout << "No value_type\n";
}

template<typename T>
void category_check() {
    if constexpr (std::is_pointer_v<T>)
        std::cout << "Category: Pointer\n";
    else if constexpr (std::is_reference_v<T>)
        std::cout << "Category: Reference\n";
    else if constexpr (std::is_array_v<T>)
        std::cout << "Category: Array\n";
    else
        std::cout << "Category: Other\n";
}

template<typename T>
void test_traits() {
    traits<T>::print();
    std::cout << "name=" << traits<T>::name << "\n---\n";
}

template<typename T>
void check_specialization() {
    std::cout << "specialization=" << traits<T>::name << "\n";
}

template<typename A, typename B>
void compare_traits() {
    std::cout << traits<A>::name << " vs " << traits<B>::name << "\n";
}

static_assert(HasValueType<std::vector<int>>);
static_assert(!HasValueType<int>);
static_assert(traits<int>::name == "int");
static_assert(traits<int*>::name == "pointer");
static_assert(traits<std::vector<int>>::name == "vector<int>");

int main() {
    std::cout << "=== Primary + full specialisations ===\n";
    traits<double>::print();
    traits<int>::print();
    traits<double*>::print();
    traits<const float>::print();
    traits<std::vector<double>>::print();
    traits<std::vector<int>>::print();

    std::cout << "\n=== test_traits ===\n";
    test_traits<int>();
    test_traits<double*>();
    test_traits<const int>();
    test_traits<std::vector<float>>();
    test_traits<std::vector<int>>();
    test_traits<int&>();
    test_traits<int&&>();

    std::cout << "\n=== Array / function type ===\n";
    int arr[5]{};
    test_traits<decltype(arr)>();
    test_traits<void(int)>();
    test_traits<std::array<int, 3>>();

    std::cout << "\n=== value_type detection ===\n";
    print_value_type_info<std::vector<int>>();
    print_value_type_info<int>();

    std::cout << "\n=== category_check ===\n";
    category_check<int*>();
    category_check<int&>();
    category_check<decltype(arr)>();
    category_check<int>();

    std::cout << "\n=== check_specialization / compare_traits ===\n";
    check_specialization<std::vector<int>>();
    check_specialization<double*>();
    compare_traits<int, double*>();

    assert(std::string_view{traits<int>::name} == "int");

    return 0;
}
