// Utilities and Helpers Exercise
// std::pair, std::tuple, std::optional, std::variant, std::any

#include <iostream>
#include <tuple>
#include <optional>
#include <variant>
#include <any>

int main() {
    // std::pair
    std::pair<int, std::string> p(42, "answer");
    std::cout << "Pair: " << p.first << ", " << p.second << "\n";
    
    // std::tuple
    auto t = std::make_tuple(1, "hello", 3.14);
    std::cout << "Tuple: " << std::get<0>(t) << ", " 
              << std::get<1>(t) << ", " << std::get<2>(t) << "\n";
    
    // std::optional
    std::optional<int> opt_val = 10;
    if (opt_val) {
        std::cout << "Optional value: " << opt_val.value() << "\n";
    }
    
    // std::variant
    std::variant<int, std::string> v = "hello";
    if (std::holds_alternative<std::string>(v)) {
        std::cout << "Variant string: " << std::get<std::string>(v) << "\n";
    }
    
    // std::any
    std::any a = 42;
    std::cout << "Any value: " << std::any_cast<int>(a) << "\n";
    
    return 0;
}
