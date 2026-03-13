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

    return 0;
}
