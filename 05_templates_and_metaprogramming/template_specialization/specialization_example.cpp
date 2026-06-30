#include <iostream>
#include <vector>
#include <type_traits>
#include <string>
#include <string_view>
#include <array>
#include <map>
#include <cassert>
#include <utility>
#include <concepts>
#include <list>

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

template<>
struct traits<double> {
    static constexpr std::string_view name = "double";
    static void print() { std::cout << "Specialized for double\n"; }
};

template<>
struct traits<bool> {
    static constexpr std::string_view name = "bool";
    static void print() { std::cout << "Specialized for bool\n"; }
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

template<typename T>
struct traits<std::list<T>> {
    static constexpr std::string_view name = "list<T>";
    static void print() { std::cout << "Specialized for list<T>\n"; }
};

template<typename K, typename V>
struct traits<std::map<K, V>> {
    static constexpr std::string_view name = "map<K,V>";
    static void print() { std::cout << "Specialized for map<K,V>\n"; }
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

template<typename T, typename U>
struct traits<std::pair<T, U>> {
    static constexpr std::string_view name = "pair<T,U>";
    static void print() { std::cout << "Specialized for pair<T,U>\n"; }
};

template<typename T>
concept HasValueType = requires { typename T::value_type; };

template<typename T>
concept HasKeyType = requires { typename T::key_type; };

template<typename T>
void print_value_type_info() {
    if constexpr (HasValueType<T>)
        std::cout << "Has value_type\n";
    else
        std::cout << "No value_type\n";
}

template<typename T>
void print_key_type_info() {
    if constexpr (HasKeyType<T>)
        std::cout << "Has key_type (associative)\n";
    else
        std::cout << "No key_type\n";
}

template<typename T>
void category_check() {
    if constexpr (std::is_pointer_v<T>)
        std::cout << "Category: Pointer\n";
    else if constexpr (std::is_reference_v<T>)
        std::cout << "Category: Reference\n";
    else if constexpr (std::is_array_v<T>)
        std::cout << "Category: Array\n";
    else if constexpr (std::is_const_v<T>)
        std::cout << "Category: Const\n";
    else if constexpr (std::is_function_v<T>)
        std::cout << "Category: Function\n";
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

template<typename T>
[[nodiscard]] constexpr bool has_custom_specialization() noexcept {
    return traits<T>::name != "Generic";
}

template<typename... Ts>
void print_all_traits() {
    (traits<Ts>::print(), ...);
}

static_assert(HasValueType<std::vector<int>>);
static_assert(!HasValueType<int>);
static_assert(HasKeyType<std::map<int, double>>);
static_assert(!HasKeyType<std::vector<int>>);
static_assert(traits<int>::name        == "int");
static_assert(traits<int*>::name       == "pointer");
static_assert(traits<std::vector<int>>::name == "vector<int>");
static_assert(has_custom_specialization<int>());
static_assert(!has_custom_specialization<float>());

int main() {
    std::cout << "=== Primary + full specialisations ===\n";
    traits<double>::print();
    traits<int>::print();
    traits<bool>::print();
    traits<double*>::print();
    traits<const float>::print();
    traits<std::vector<double>>::print();
    traits<std::vector<int>>::print();
    traits<std::list<int>>::print();
    traits<std::map<int, std::string>>::print();
    traits<std::pair<int, double>>::print();

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

    std::cout << "\n=== value_type / key_type detection ===\n";
    print_value_type_info<std::vector<int>>();
    print_value_type_info<int>();
    print_key_type_info<std::map<int, double>>();
    print_key_type_info<std::vector<int>>();

    std::cout << "\n=== category_check ===\n";
    category_check<int*>();
    category_check<int&>();
    category_check<decltype(arr)>();
    category_check<int>();
    category_check<void(int)>();

    std::cout << "\n=== check_specialization / compare_traits ===\n";
    check_specialization<std::vector<int>>();
    check_specialization<double*>();
    compare_traits<int, double*>();

    std::cout << "\n=== has_custom_specialization ===\n";
    std::cout << std::boolalpha
              << "int:   "   << has_custom_specialization<int>()   << "\n"
              << "float: "   << has_custom_specialization<float>() << "\n";

    std::cout << "\n=== print_all_traits (variadic) ===\n";
    print_all_traits<int, double, bool>();

    assert(std::string_view{traits<int>::name} == "int");
    assert(has_custom_specialization<int>());
    assert(!has_custom_specialization<float>());

    std::cout << "\nAll assertions passed.\n";
    return 0;
}
