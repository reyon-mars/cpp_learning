// Concepts, Ranges, Coroutines (C++20) Exercise
// Next generation C++ features

#include <iostream>
#include <concepts>
#include <ranges>
#include <vector>
#include <string>

template<typename T>
concept Printable = requires(std::ostream& os, T t) {
    { os << t };
};

template<Printable T>
void print(const T& t) {
    std::cout << t << "\n";
}

// ----- Small additional concept -----
template<typename T>
concept Number = std::integral<T> || std::floating_point<T>;

template<Number T>
T add(T a, T b) {
    return a + b;
}
// ------------------------------------

// ===== VERY SMALL NEW CONCEPT =====
template<typename T>
concept Container = requires(T a) {
    a.begin();
    a.end();
};
// ==================================

int main() {
    print(42);
    print("hello");
    print(3.14);
    
    // ---- Small concept usage ----
    std::cout << "Add ints: " << add(5, 3) << "\n";
    std::cout << "Add doubles: " << add(2.5, 4.1) << "\n";
    // -----------------------------

    // Ranges
    std::vector<int> vec = {1, 2, 3, 4, 5};
    auto evens = vec | std::views::filter([](int x) { return x % 2 == 0; });
    
    std::cout << "Evens: ";
    for (int v : evens) {
        std::cout << v << " ";
    }
    std::cout << "\n";

    // ----- Additional ranges examples -----

    // Transform view (square numbers)
    auto squares = vec | std::views::transform([](int x) { return x * x; });

    std::cout << "Squares: ";
    for (int v : squares) {
        std::cout << v << " ";
    }
    std::cout << "\n";

    // Take first 3 elements
    auto first_three = vec | std::views::take(3);

    std::cout << "First three: ";
    for (int v : first_three) {
        std::cout << v << " ";
    }
    std::cout << "\n";

    // -------------------------------------

    // ===== VERY SMALL NEW ADDITIONS =====

    // Drop first 2 elements
    auto after_two = vec | std::views::drop(2);
    std::cout << "After two: ";
    for (int v : after_two)
        std::cout << v << " ";
    std::cout << "\n";

    // Reverse view
    auto reversed = vec | std::views::reverse;
    std::cout << "Reversed: ";
    for (int v : reversed)
        std::cout << v << " ";
    std::cout << "\n";

    // Use Container concept (simple check)
    if constexpr (Container<std::vector<int>>) {
        std::cout << "Vector satisfies Container concept\n";
    }

    // ===================================

    return 0;
}
