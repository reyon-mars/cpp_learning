// Structured Bindings, Optional, Variant (C++17) Exercise
// Modern ways to work with data

#include <iostream>
#include <tuple>
#include <optional>
#include <variant>
#include <string>

// ---- tiny addition ----
#include <vector>
#include <numeric>
#include <algorithm> // added
// -----------------------

std::optional<int> safe_divide(int a, int b) {
    if (b == 0) return std::nullopt;
    return a / b;
}

// ---- VERY SMALL EXTRA HELPERS ----

// check if optional has value
bool has_value(const std::optional<int>& opt) {
    return opt.has_value();
}

// sum vector helper
int sum_vector(const std::vector<int>& v) {
    return std::accumulate(v.begin(), v.end(), 0);
}

// print divider
void print_divider() {
    std::cout << "----------------------\n";
}

// ----------------------------------

// ===== EXTRA SMALL HELPERS =====

// average helper
double average_vector(const std::vector<int>& v) {
    if (v.empty()) return 0.0;
    return static_cast<double>(sum_vector(v)) / v.size();
}

// check if vector contains value
bool contains_value(const std::vector<int>& v, int value) {
    return std::find(v.begin(), v.end(), value) != v.end();
}

// print vector
void print_vector(const std::vector<int>& v) {
    for (int n : v)
        std::cout << n << " ";
    std::cout << "\n";
}

// =================================

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

    // ===== EXTRA SMALL ADDITIONS =====

    print_divider();

    // optional check helper
    auto test_opt = safe_divide(5, 1);
    std::cout << "Optional has value? "
              << (has_value(test_opt) ? "Yes" : "No") << "\n";

    // vector + accumulate demo
    std::vector<int> nums = {1, 2, 3, 4};
    std::cout << "Sum of nums: " << sum_vector(nums) << "\n";

    // variant index info
    std::cout << "Variant index: " << v.index() << "\n";

    // ==================================

    // ===== FINAL TINY ADDITIONS =====

    // print vector contents
    std::cout << "Vector contents: ";
    print_vector(nums);

    // average calculation
    std::cout << "Average of nums: "
              << average_vector(nums) << "\n";

    // min and max element
    auto [min_it, max_it] =
        std::minmax_element(nums.begin(), nums.end());

    std::cout << "Min: " << *min_it
              << ", Max: " << *max_it << "\n";

    // contains value check
    std::cout << "Contains 3? "
              << (contains_value(nums, 3) ? "Yes" : "No")
              << "\n";

    // reverse vector copy
    std::vector<int> reversed = nums;
    std::reverse(reversed.begin(), reversed.end());

    std::cout << "Reversed nums: ";
    print_vector(reversed);

    // ==================================

    return 0;
}
