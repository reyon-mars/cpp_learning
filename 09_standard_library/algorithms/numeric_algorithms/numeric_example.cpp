// Numeric Algorithms Exercise
// accumulate, inner_product, adjacent_difference, partial_sum

#include <iostream>
#include <vector>
#include <numeric>

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    
    // accumulate (sum)
    int sum = std::accumulate(vec.begin(), vec.end(), 0);
    std::cout << "Sum: " << sum << "\n";
    
    // accumulate with custom operation
    int product = std::accumulate(vec.begin(), vec.end(), 1,
                                  [](int a, int b) { return a * b; });
    std::cout << "Product: " << product << "\n";
    
    // inner_product
    std::vector<int> vec2 = {2, 3, 4, 5, 6};
    int dot_product = std::inner_product(vec.begin(), vec.end(), 
                                         vec2.begin(), 0);
    std::cout << "Dot product: " << dot_product << "\n";
    
    return 0;
}
