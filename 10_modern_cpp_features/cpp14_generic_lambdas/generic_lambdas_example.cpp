// Generic Lambdas (C++14) Exercise
// Type-deducing lambda parameters

#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> ints = {1, 2, 3, 4, 5};
    std::vector<double> doubles = {1.1, 2.2, 3.3};
    
    // Generic lambda with auto parameter
    auto print_double = [](auto x) {
        std::cout << (x * 2) << " ";
    };
    
    std::cout << "Doubled ints: ";
    std::for_each(ints.begin(), ints.end(), print_double);
    std::cout << "\n";
    
    std::cout << "Doubled doubles: ";
    std::for_each(doubles.begin(), doubles.end(), print_double);
    std::cout << "\n";

    // ---- Additional small generic lambda examples ----

    // Generic sum lambda
    auto add = [](auto a, auto b) {
        return a + b;
    };

    std::cout << "Sum (int): " << add(5, 3) << "\n";
    std::cout << "Sum (double): " << add(2.5, 4.1) << "\n";

    // Generic comparison lambda
    auto is_greater = [](auto a, auto b) {
        return a > b;
    };

    std::cout << "Is 10 > 5? "
              << (is_greater(10, 5) ? "Yes" : "No") << "\n";

    // Generic container printer
    auto print_container = [](const auto& container) {
        for (const auto& item : container)
            std::cout << item << " ";
        std::cout << "\n";
    };

    std::cout << "Ints: ";
    print_container(ints);

    std::cout << "Doubles: ";
    print_container(doubles);

    // --------------------------------------------------

    // ===== VERY SMALL NEW ADDITIONS =====

    // Generic lambda for squaring values
    auto square = [](auto x) {
        return x * x;
    };

    std::cout << "Square of 4: " << square(4) << "\n";
    std::cout << "Square of 2.5: " << square(2.5) << "\n";

    // Generic lambda with capture
    int factor = 3;
    auto multiply_by_factor = [factor](auto x) {
        return x * factor;
    };

    std::cout << "Multiply 5 by factor: "
              << multiply_by_factor(5) << "\n";

    // Use generic lambda with transform
    std::vector<int> squared(ints.size());
    std::transform(ints.begin(), ints.end(), squared.begin(), square);

    std::cout << "Squared ints: ";
    print_container(squared);

    // Generic predicate lambda
    auto is_even = [](auto x) {
        return x % 2 == 0;
    };

    std::cout << "Even numbers: ";
    for (auto x : ints)
        if (is_even(x))
            std::cout << x << " ";
    std::cout << "\n";

    // ===================================

    return 0;
}
