// Structured Bindings, Optional, Variant (C++17) Exercise
// Modern ways to work with data

#include <iostream>
#include <tuple>
#include <optional>
#include <variant>

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
    
    return 0;
}
