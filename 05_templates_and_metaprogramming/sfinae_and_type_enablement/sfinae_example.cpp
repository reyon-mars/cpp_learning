// SFINAE and Type Enablement Exercise
// Substitution Failure Is Not An Error

#include <iostream>
#include <type_traits>

// SFINAE approach
template<typename T>
typename std::enable_if<std::is_integral_v<T>>::type
process(T value) {
    std::cout << "Processing integral: " << value << "\n";
}

template<typename T>
typename std::enable_if<std::is_floating_point_v<T>>::type
process(T value) {
    std::cout << "Processing float: " << value << "\n";
}

// Alternative: requires (C++20 concepts preferred)
template<typename T>
concept Arithmetic = std::is_arithmetic_v<T>;

template<Arithmetic T>
void process_v2(T value) {
    std::cout << "Process v2: " << value << "\n";
}

int main() {
    process(42);
    process(3.14);
    
    process_v2(100);
    process_v2(2.71);
    
    return 0;
}
