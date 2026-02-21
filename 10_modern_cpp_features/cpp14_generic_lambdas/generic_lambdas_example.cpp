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
    
    return 0;
}
