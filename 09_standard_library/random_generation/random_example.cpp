// Random Number Generation Exercise
// std::random_device, engines, distributions

#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <numeric> // tiny addition

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

    // ---- Additional small examples ----

    // Bernoulli distribution (true/false)
    std::bernoulli_distribution coin(0.5);
    std::cout << "Coin toss: " << (coin(rng) ? "Heads" : "Tails") << "\n";

    // Generate random vector
    std::vector<int> numbers(5);
    for (auto& n : numbers) {
        n = dist_int(rng);
    }

    std::cout << "Random vector: ";
    for (int n : numbers) {
        std::cout << n << " ";
    }
    std::cout << "\n";

    // Shuffle the vector
    std::shuffle(numbers.begin(), numbers.end(), rng);
    std::cout << "Shuffled vector: ";
    for (int n : numbers) {
        std::cout << n << " ";
    }
    std::cout << "\n";

    // ---- VERY SMALL EXTRA ADDITIONS ----

    // Find min and max in random vector
    auto [min_it, max_it] = std::minmax_element(numbers.begin(), numbers.end());
    std::cout << "Min: " << *min_it << ", Max: " << *max_it << "\n";

    // Sum of elements
    int sum = std::accumulate(numbers.begin(), numbers.end(), 0);
    std::cout << "Sum of vector: " << sum << "\n";

    // Generate one random boolean sequence
    std::cout << "Random bools: ";
    for (int i = 0; i < 5; ++i) {
        std::cout << coin(rng) << " ";
    }
    std::cout << "\n";

    // Reseed example (deterministic behavior)
    rng.seed(42);
    std::cout << "After reseed (42): " << dist_int(rng) << "\n";

    // -----------------------------------
    // EXTRA SMALL ADDITIONS (NEW)
    // -----------------------------------

    // Generate numbers using generate()
    std::vector<int> generated(5);
    std::generate(generated.begin(), generated.end(),
                  [&]() { return dist_int(rng); });

    std::cout << "Generated vector: ";
    for (int n : generated) std::cout << n << " ";
    std::cout << "\n";

    // Discrete distribution
    std::discrete_distribution<int> discrete({1, 2, 3});
    std::cout << "Discrete distribution sample: "
              << discrete(rng) << "\n";

    // Uniform real in different range
    std::uniform_real_distribution<double> dist_real2(10.0, 20.0);
    std::cout << "Random [10,20]: " << dist_real2(rng) << "\n";

    // Check average of generated vector
    double avg = generated.empty() ? 0.0 :
        static_cast<double>(std::accumulate(generated.begin(), generated.end(), 0)) / generated.size();

    std::cout << "Average of generated vector: " << avg << "\n";

    return 0;
}
