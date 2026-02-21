// Variadic Templates Exercise
// Parameter packs and fold expressions

#include <iostream>

// Recursive case
template<typename T>
void print_all(const T& first) {
    std::cout << first << "\n";
}

// Variadic case
template<typename T, typename... Args>
void print_all(const T& first, const Args&... rest) {
    std::cout << first << " ";
    print_all(rest...);
}

// Fold expressions (C++17)
template<typename... Args>
auto sum(Args... args) {
    return (args + ...);  // Left fold
}

int main() {
    print_all(1, 2.5, "hello", 3, 4);
    std::cout << "\n";
    
    std::cout << "Sum: " << sum(1, 2, 3, 4, 5) << "\n";
    
    return 0;
}
