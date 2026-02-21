// Async Programming Exercise
// std::async and packaged_task

#include <iostream>
#include <future>

int heavy_computation(int n) {
    int sum = 0;
    for (int i = 0; i <= n; ++i) {
        sum += i;
    }
    return sum;
}

int main() {
    // Using std::async
    auto future1 = std::async(std::launch::async, heavy_computation, 1000);
    auto future2 = std::async(std::launch::async, heavy_computation, 2000);
    
    std::cout << "Computations running asynchronously...\n";
    
    int result1 = future1.get();
    int result2 = future2.get();
    
    std::cout << "Result 1: " << result1 << "\n";
    std::cout << "Result 2: " << result2 << "\n";
    
    return 0;
}
