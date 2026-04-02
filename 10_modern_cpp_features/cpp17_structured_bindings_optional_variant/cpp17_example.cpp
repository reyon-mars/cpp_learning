// Structured Bindings, Optional, Variant (C++17) Exercise
// Modern ways to work with data

#include <iostream>
#include <tuple>
#include <optional>
#include <variant>
#include <string>

std::optional<int> safe_divide(int a, int b) {
    if (b == 0) return std::nullopt;
    return a / b;
}

int main() {
    // Structured bindings
    auto [x, y] = std::make_tuple(10, 20);
    std::cout << "x: " << x << ", y: " << y << "\n";
    
    // Optional
    if (auto result = safe_divide(10, 2)) {
        std::cout << "Result: " << result.value() << "\n";
    }
    
    if (auto result = safe_divide(10, 0)) {
        std::cout << "Result: " << result.value() << "\n";
    } else {
        std::cout << "Division by zero\n";
    }
    
    // Variant
    std::variant<int, std::string> v = "hello";
    if (std::holds_alternative<std::string>(v)) {
        std::cout << "String: " << std::get<std::string>(v) << "\n";
    }

    // ---- Additional small examples ----

    // Another structured binding example
    std::pair<int, int> p{5, 7};
    auto [a, b] = p;
    std::cout << "Pair values: " << a << ", " << b << "\n";

    // Optional with fallback value
    auto val = safe_divide(20, 4).value_or(0);
    std::cout << "Safe result with fallback: " << val << "\n";

    // Change variant type
    v = 42;
    if (std::holds_alternative<int>(v)) {
        std::cout << "Variant int: " << std::get<int>(v) << "\n";
    }

    // Visit variant (modern approach)
    v = std::string("C++17");
    std::visit([](auto&& arg) {
        std::cout << "Visited variant: " << arg << "\n";
    }, v);

    // -----------------------------------

    return 0;
}

