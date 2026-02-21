// Function Wrappers Exercise
// std::function and type erasure

#include <iostream>
#include <functional>
#include <vector>

int add(int a, int b) { return a + b; }

struct Multiplier {
    int factor;
    int operator()(int x) const { return x * factor; }
};

int main() {
    // std::function can hold any callable
    std::vector<std::function<int()>> functions;
    
    int value = 10;
    functions.push_back([value]() { return value + 5; });
    functions.push_back([value]() { return value * 2; });
    functions.push_back([value, multiplier = Multiplier{3}]() { 
        return multiplier(value); 
    });
    
    for (const auto& f : functions) {
        std::cout << "Result: " << f() << "\n";
    }
    
    // std::bind
    auto add_10 = std::bind(add, std::placeholders::_1, 10);
    std::cout << "Bound result: " << add_10(5) << "\n";
    
    return 0;
}
