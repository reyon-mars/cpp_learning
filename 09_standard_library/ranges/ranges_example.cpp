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

    // ---- Additional small examples ----

    // take first 5 elements
    auto first_five = vec | std::views::take(5);
    std::cout << "First five: ";
    for (int v : first_five) {
        std::cout << v << " ";
    }
    std::cout << "\n";

    // drop first 5 elements
    auto after_five = vec | std::views::drop(5);
    std::cout << "After five: ";
    for (int v : after_five) {
        std::cout << v << " ";
    }
    std::cout << "\n";

    // reverse view
    auto reversed = vec | std::views::reverse;
    std::cout << "Reversed: ";
    for (int v : reversed) {
        std::cout << v << " ";
    }
    std::cout << "\n";

    // -----------------------------------

    return 0;
}
