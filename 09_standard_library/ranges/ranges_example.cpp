// Ranges Exercise (C++20)
// Range-based algorithms and views

#include <iostream>
#include <vector>
#include <ranges>

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    // Range-based sort
    auto evens = vec | std::views::filter([](int x) { return x % 2 == 0; });
    
    std::cout << "Even numbers: ";
    for (int v : evens) {
        std::cout << v << " ";
    }
    std::cout << "\n";
    
    // Range-based transform
    auto doubled = vec | std::views::transform([](int x) { return x * 2; });
    
    std::cout << "Doubled: ";
    for (int v : doubled) {
        std::cout << v << " ";
    }
    std::cout << "\n";
    
    return 0;
}
