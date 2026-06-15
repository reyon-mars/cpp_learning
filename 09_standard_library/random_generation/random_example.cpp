#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <numeric>
#include <span>

void print_vec(std::span<const int> v, const char* label = nullptr) {
    if (label) std::cout << label << ": ";
    for (int n : v) std::cout << n << " ";
    std::cout << "\n";
}

void print_stats(std::span<const int> v) {
    if (v.empty()) return;
    const int   sum  = std::accumulate(v.begin(), v.end(), 0);
    const auto [lo, hi] = std::ranges::minmax(v);
    const double avg    = static_cast<double>(sum) / static_cast<double>(v.size());
    const long evens    = std::ranges::count_if(v, [](int x){ return x % 2 == 0; });
    std::cout << "sum=" << sum << " min=" << lo << " max=" << hi
              << " avg=" << avg << " evens=" << evens << "\n";
}

int main() {
    std::mt19937 rng{std::random_device{}()};

    std::uniform_int_distribution<int>    dist_int {1,    10};
    std::uniform_real_distribution<double> dist_real{0.0,  1.0};
    std::normal_distribution<double>       dist_norm{0.0,  1.0};
    std::bernoulli_distribution            coin     {0.5};
    std::discrete_distribution<int>        discrete {{1, 2, 3}};
    std::uniform_int_distribution<int>    char_dist{65,   90};
    std::uniform_real_distribution<double> dist_real2{10.0, 20.0};

    std::cout << "=== Uniform int [1,10] ===\n";
    std::cout << "Samples: ";
    for (int i = 0; i < 5; ++i) std::cout << dist_int(rng) << " ";
    std::cout << "\n";

    std::cout << "\n=== Uniform real [0,1] ===\n";
    std::cout << "Samples: ";
    for (int i = 0; i < 5; ++i) std::cout << dist_real(rng) << " ";
    std::cout << "\n";

    std::cout << "\n=== Normal (mean=0, sd=1) ===\n";
    std::cout << "Sample=" << dist_norm(rng) << "\n";

    std::cout << "\n=== Bernoulli (coin) ===\n";
    std::cout << "Toss=" << (coin(rng) ? "Heads" : "Tails") << "\n";
    std::cout << "Bools: ";
    for (int i = 0; i < 5; ++i) std::cout << std::boolalpha << coin(rng) << " ";
    std::cout << "\n";

    std::cout << "\n=== Discrete {weights 1,2,3} ===\n";
    std::cout << "Sample=" << discrete(rng) << "\n";

    std::cout << "\n=== Uniform real [10,20] ===\n";
    std::cout << "Sample=" << dist_real2(rng) << "\n";

    std::cout << "\n=== Random uppercase chars ===\n";
    for (int i = 0; i < 5; ++i)
        std::cout << static_cast<char>(char_dist(rng)) << " ";
    std::cout << "\n";

    std::cout << "\n=== Random vector + shuffle ===\n";
    std::vector<int> numbers(5);
    std::ranges::generate(numbers, [&]{ return dist_int(rng); });
    print_vec(numbers, "Generated");
    print_stats(numbers);

    std::ranges::shuffle(numbers, rng);
    print_vec(numbers, "Shuffled");

    std::ranges::sort(numbers);
    print_vec(numbers, "Sorted");

    std::ranges::reverse(numbers);
    print_vec(numbers, "Reversed");

    std::cout << "\n=== Reseed (deterministic) ===\n";
    rng.seed(42);
    std::vector<int> seeded(5);
    std::ranges::generate(seeded, [&]{ return dist_int(rng); });
    print_vec(seeded, "After seed(42)");
    print_stats(seeded);

    return 0;
}
