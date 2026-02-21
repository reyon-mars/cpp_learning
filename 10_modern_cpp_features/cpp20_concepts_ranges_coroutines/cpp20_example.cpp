// Concepts, Ranges, Coroutines (C++20) Exercise
// Next generation C++ features

#include <iostream>
#include <concepts>
#include <ranges>

template<typename T>
concept Printable = requires(std::ostream& os, T t) {
    { os << t };
};

template<Printable T>
void print(const T& t) {
    std::cout << t << "\n";
}

int main() {
    print(42);
    print("hello");
    print(3.14);
    
    // Ranges
    std::vector<int> vec = {1, 2, 3, 4, 5};
    auto evens = vec | std::views::filter([](int x) { return x % 2 == 0; });
    
    std::cout << "Evens: ";
    for (int v : evens) {
        std::cout << v << " ";
    }
    std::cout << "\n";
    
    return 0;
}
