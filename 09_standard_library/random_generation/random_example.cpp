// Random Number Generation Exercise
// std::random_device, engines, distributions

#include <iostream>
#include <random>
#include <vector>

int main() {
    // Random engine
    std::mt19937 rng(std::random_device{}());
    
    // Uniform distribution
    std::uniform_int_distribution<int> dist_int(1, 10);
    std::uniform_real_distribution<double> dist_real(0.0, 1.0);
    
    std::cout << "Random integers: ";
    for (int i = 0; i < 5; ++i) {
        std::cout << dist_int(rng) << " ";
    }
    std::cout << "\n";
    
    std::cout << "Random doubles: ";
    for (int i = 0; i < 5; ++i) {
        std::cout << dist_real(rng) << " ";
    }
    std::cout << "\n";
    
    // Normal distribution
    std::normal_distribution<double> dist_normal(0.0, 1.0);
    std::cout << "Normal distribution: " << dist_normal(rng) << "\n";
    
    return 0;
}
