// SFINAE and Type Enablement Exercise
// Substitution Failure Is Not An Error

#include <iostream>
#include <type_traits>
#include <vector>

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

// ----------------------------------
// Main
// ----------------------------------
int main() {

    process(42);
    process(3.14);

    int x = 10;
    process_pointer(&x);

    std::vector<int> vec = {1, 2, 3};
    process_container(vec);

    process_v2(100);
    process_v2(2.71);

    return 0;
}
