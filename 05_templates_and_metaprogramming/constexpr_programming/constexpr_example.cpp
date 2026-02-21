// Constexpr Programming Exercise
// Compile-time computation and evaluation

#include <iostream>

constexpr int factorial(int n) {
    return n <= 1 ? 1 : n * factorial(n - 1);
}

constexpr int fibonacci(int n) {
    return n <= 1 ? n : fibonacci(n - 1) + fibonacci(n - 2);
}

class ConstexprArray {
private:
    int data[10];
    
public:
    constexpr ConstexprArray() : data{} {}
    
    constexpr void set(int index, int value) {
        data[index] = value;
    }
    
    constexpr int get(int index) const {
        return data[index];
    }
};

int main() {
    // Compile-time computation
    constexpr int fact5 = factorial(5);
    constexpr int fib10 = fibonacci(10);
    
    std::cout << "5! = " << fact5 << "\n";
    std::cout << "fib(10) = " << fib10 << "\n";
    
    // Runtime computation
    int n = 6;
    std::cout << n << "! = " << factorial(n) << "\n";
    
    return 0;
}
