// Higher-Order Functions Exercise
// Functions that work with other functions

#include <iostream>
#include <vector>
#include <algorithm>

int add(int a, int b) { return a + b; }
int multiply(int a, int b) { return a * b; }

// Higher-order function: applies a binary operation
template<typename BinOp>
int reduce(const std::vector<int>& values, BinOp op) {
    int result = values[0];
    for (size_t i = 1; i < values.size(); ++i) {
        result = op(result, values[i]);
    }
    return result;
}

int main() {
    std::vector<int> values = {1, 2, 3, 4, 5};
    
    int sum = reduce(values, add);
    int product = reduce(values, multiply);
    
    std::cout << "Sum: " << sum << "\n";
    std::cout << "Product: " << product << "\n";
    
    return 0;
}
