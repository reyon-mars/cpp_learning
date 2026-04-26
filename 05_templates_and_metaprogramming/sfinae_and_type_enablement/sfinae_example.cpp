// SFINAE and Type Enablement Exercise
// Substitution Failure Is Not An Error

#include <iostream>
#include <type_traits>
#include <vector>
#include <string>   // 🔹 ADDED
#include <iterator> // 🔹 ADDED

// ----------------------------------
// SFINAE via return type
// ----------------------------------
template<typename T>
std::enable_if_t<std::is_integral_v<T>>
process(T value) {
    std::cout << "Processing integral: " << value << "\n";
}

template<typename T>
std::enable_if_t<std::is_floating_point_v<T>>
process(T value) {
    std::cout << "Processing float: " << value << "\n";
}

// ----------------------------------
// SFINAE via template parameter
// ----------------------------------
template<typename T,
         typename = std::enable_if_t<std::is_pointer_v<T>>>
void process_pointer(T value) {
    std::cout << "Processing pointer: " << value << "\n";
}

// ----------------------------------
// Detection Idiom (check for .size())
// ----------------------------------
template<typename, typename = std::void_t<>>
struct has_size : std::false_type {};

template<typename T>
struct has_size<T, std::void_t<decltype(std::declval<T>().size())>>
    : std::true_type {};

template<typename T>
std::enable_if_t<has_size<T>::value>
process_container(const T& value) {
    std::cout << "Container size: " << value.size() << "\n";
}

// ----------------------------------
// C++20 Concept Alternative
// ----------------------------------
template<typename T>
concept Arithmetic = std::is_arithmetic_v<T>;

template<Arithmetic T>
void process_v2(T value) {
    std::cout << "Process v2: " << value << "\n";
}

// ---------------- SMALL ADDITIONS ----------------

template<typename T>
std::enable_if_t<!std::is_arithmetic_v<T> && !std::is_pointer_v<T>>
process_generic(const T&) {
    std::cout << "Processing generic type\n";
}

template<typename T>
void print_type_info() {
    std::cout << "Is integral: " << std::is_integral_v<T> << "\n";
    std::cout << "Is floating: " << std::is_floating_point_v<T> << "\n";
    std::cout << "Is pointer: " << std::is_pointer_v<T> << "\n";
}

template<typename T>
void safe_process(T value) {
    if constexpr (std::is_integral_v<T>)
        process(value);
    else if constexpr (std::is_floating_point_v<T>)
        process(value);
    else
        process_generic(value);
}

// ======================================================
// 🔥 NEW ADDITIONS
// ======================================================

// 🔹 SFINAE via default template argument (classic)
template<typename T, typename = void>
struct is_streamable : std::false_type {};

template<typename T>
struct is_streamable<T,
    std::void_t<decltype(std::declval<std::ostream&>() << std::declval<T>())>>
    : std::true_type {};

// 🔹 Overload only if streamable
template<typename T>
std::enable_if_t<is_streamable<T>::value>
print_streamable(const T& value) {
    std::cout << "Streamable: " << value << "\n";
}

// 🔹 Detect iterable (has begin/end)
template<typename, typename = std::void_t<>>
struct is_iterable : std::false_type {};

template<typename T>
struct is_iterable<T,
    std::void_t<
        decltype(std::begin(std::declval<T>())),
        decltype(std::end(std::declval<T>()))
    >> : std::true_type {};

// 🔹 Process iterable types
template<typename T>
std::enable_if_t<is_iterable<T>::value>
process_iterable(const T& container) {
    std::cout << "Iterable elements: ";
    for (const auto& x : container)
        std::cout << x << " ";
    std::cout << "\n";
}

// 🔹 Hybrid: if constexpr + SFINAE
template<typename T>
void smart_process(const T& value) {
    if constexpr (is_iterable<T>::value) {
        process_iterable(value);
    } else if constexpr (std::is_arithmetic_v<T>) {
        process_v2(value);
    } else {
        std::cout << "Unknown type\n";
    }
}

// 🔹 C++20 requires-expression version
template<typename T>
requires requires(T t) { t.size(); }
void process_requires(const T& value) {
    std::cout << "Requires size(): " << value.size() << "\n";
}

// 🔹 type_identity trick (prevents deduction)
template<typename T>
void no_deduction(typename std::type_identity<T>::type value) {
    std::cout << "No deduction type used\n";
}

// ======================================================
// Main
// ======================================================
int main() {

    process(42);
    process(3.14);

    int x = 10;
    process_pointer(&x);

    std::vector<int> vec = {1, 2, 3};
    process_container(vec);

    process_v2(100);
    process_v2(2.71);

    std::cout << "\n--- Extra Tests ---\n";

    safe_process(50);
    safe_process(5.5);
    safe_process(std::string("hello"));

    print_type_info<int>();
    print_type_info<double>();
    print_type_info<int*>();

    // ======================================================
    // 🔥 NEW USAGE
    // ======================================================

    std::cout << "\n--- Advanced SFINAE Tests ---\n";

    print_streamable(123);
    print_streamable(std::string("SFINAE"));

    process_iterable(vec);

    smart_process(42);
    smart_process(vec);
    smart_process(std::string("Hi"));

    process_requires(vec);

    no_deduction<int>(10);

    // ======================================================

    return 0;
}
