// Compile-time Programming Experiment
// Exploring constexpr limits and compile-time computation

#include <iostream>
#include <array>

constexpr int fibonacci_ct(int n) {
    return n <= 1 ? n : fibonacci_ct(n - 1) + fibonacci_ct(n - 2);
}

constexpr std::array<int, 10> generate_sequence() {
    std::array<int, 10> seq{};
    for (int i = 0; i < 10; ++i) {
        seq[i] = i * i;
    }
    return seq;
}

int main() {
    constexpr int fib15 = fibonacci_ct(15);
    constexpr auto sequence = generate_sequence();
    
    std::cout << "Fib(15) at compile-time: " << fib15 << "\n";
    std::cout << "Sequence: ";
    for (int v : sequence) {
        std::cout << v << " ";
    }
    std::cout << "\n";
    
    return 0;
}
