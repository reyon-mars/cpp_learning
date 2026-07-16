#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <numeric>
#include <span>
#include <string>
#include <string_view>
#include <map>
#include <format>
#include <ranges>
#include <cassert>
#include <array>
#include <cmath>

void print_vec(std::span<const int> v, std::string_view label = {}) {
    if (!label.empty()) std::cout << label << ": ";
    for (const int n : v) std::cout << n << ' ';
    std::cout << '\n';
}

void print_stats(std::span<const int> v) {
    if (v.empty()) return;
    const int    sum        = std::reduce(v.begin(), v.end(), 0);
    const auto  [lo, hi]   = std::ranges::minmax(v);
    const double avg        = static_cast<double>(sum) / static_cast<double>(v.size());
    const long   evens      = std::ranges::count_if(v, [](int x){ return x % 2 == 0; });
    std::cout << std::format("sum={} min={} max={} avg={} evens={}\n",
                             sum, lo, hi, avg, evens);
}

void print_stats(std::span<const double> v) {
    if (v.empty()) return;
    const double sum = std::reduce(v.begin(), v.end(), 0.0);
    const auto  [lo, hi] = std::ranges::minmax(v);
    const double avg = sum / static_cast<double>(v.size());
    std::cout << std::format("sum={:.4f} min={:.4f} max={:.4f} avg={:.4f}\n",
                             sum, lo, hi, avg);
}

void print_histogram(std::span<const int> v, int bins = 5, int bar_width = 20) {
    if (v.empty()) return;
    const auto [lo, hi] = std::ranges::minmax(v);
    const double bin_size = (hi - lo + 1.0) / bins;

    std::vector<int> counts(bins, 0);
    for (const int x : v) {
        const int bin = std::min(static_cast<int>((x - lo) / bin_size), bins - 1);
        ++counts[bin];
    }

    const int max_count = *std::ranges::max_element(counts);
    for (int i = 0; i < bins; ++i) {
        const double lo_edge = lo + i * bin_size;
        const double hi_edge = lo_edge + bin_size;
        const int    bar_len = max_count > 0
                               ? (counts[i] * bar_width) / max_count : 0;
        std::cout << std::format("[{:5.1f},{:5.1f}) |{:<{}}| {}\n",
                                 lo_edge, hi_edge,
                                 std::string(bar_len, '#'), bar_width,
                                 counts[i]);
    }
}

struct Rng {
    explicit Rng(std::uint32_t seed = std::random_device{}()) noexcept
        : engine{seed} {}

    void reseed(std::uint32_t seed) noexcept { engine.seed(seed); }

    template <typename Dist>
    auto operator()(Dist& dist) { return dist(engine); }

    template <typename Dist>
    auto generate(Dist& dist, std::size_t n) {
        using T = decltype(dist(engine));
        std::vector<T> out(n);
        std::ranges::generate(out, [&]{ return dist(engine); });
        return out;
    }

    std::mt19937 engine;
};

int main() {
    Rng rng;

    std::uniform_int_distribution<int>     dist_int  {1,    10};
    std::uniform_real_distribution<double> dist_real {0.0,  1.0};
    std::normal_distribution<double>       dist_norm {0.0,  1.0};
    std::bernoulli_distribution            coin      {0.5};
    std::discrete_distribution<int>        discrete  {{1, 2, 3}};
    std::uniform_int_distribution<int>     char_dist {65,   90};
    std::uniform_real_distribution<double> dist_real2{10.0, 20.0};

    std::cout << "=== Uniform int [1,10] ===\n";
    std::cout << "Samples: ";
    for (int i = 0; i < 5; ++i) std::cout << rng(dist_int) << ' ';
    std::cout << '\n';

    std::cout << "\n=== Uniform real [0,1] ===\n";
    std::cout << "Samples: ";
    for (int i = 0; i < 5; ++i) std::cout << rng(dist_real) << ' ';
    std::cout << '\n';

    std::cout << "\n=== Normal (mean=0, sd=1) ===\n";
    std::cout << std::format("Sample={:.5f}\n", rng(dist_norm));

    std::cout << "\n=== Bernoulli (coin) ===\n";
    std::cout << (rng(coin) ? "Toss=Heads\n" : "Toss=Tails\n");
    std::cout << "Bools: ";
    for (int i = 0; i < 5; ++i) std::cout << std::boolalpha << rng(coin) << ' ';
    std::cout << '\n';

    std::cout << "\n=== Discrete {weights 1,2,3} ===\n";
    std::cout << std::format("Sample={}\n", rng(discrete));

    std::cout << "\n=== Uniform real [10,20] ===\n";
    std::cout << std::format("Sample={:.4f}\n", rng(dist_real2));

    std::cout << "\n=== Random uppercase chars ===\n";
    for (int i = 0; i < 5; ++i)
        std::cout << static_cast<char>(rng(char_dist)) << ' ';
    std::cout << '\n';

    std::cout << "\n=== Random vector + shuffle ===\n";
    auto numbers = rng.generate(dist_int, 5);
    print_vec(numbers, "Generated");
    print_stats(numbers);
    std::ranges::shuffle(numbers, rng.engine);
    print_vec(numbers, "Shuffled");
    std::ranges::sort(numbers);
    print_vec(numbers, "Sorted");
    std::ranges::reverse(numbers);
    print_vec(numbers, "Reversed");

    std::cout << "\n=== Reseed (deterministic) ===\n";
    rng.reseed(42);
    auto seeded = rng.generate(dist_int, 5);
    print_vec(seeded, "After seed(42)");
    print_stats(seeded);

    assert((seeded == std::vector<int>{4, 8, 10, 2, 8}));

    std::cout << "\n=== Geometric distribution ===\n";
    std::geometric_distribution<int> dist_geo{0.3};
    const auto geo_samples = rng.generate(dist_geo, 10);
    print_vec(geo_samples, "Geometric(0.3)");
    print_stats(geo_samples);

    std::cout << "\n=== Poisson distribution ===\n";
    std::poisson_distribution<int> dist_poisson{4.0};
    const auto poisson_samples = rng.generate(dist_poisson, 10);
    print_vec(poisson_samples, "Poisson(lambda=4)");
    print_stats(poisson_samples);

    std::cout << "\n=== Histogram (1000 uniform int samples) ===\n";
    const auto big = rng.generate(dist_int, 1000);
    print_histogram(big, 5, 30);

    std::cout << "\n=== Normal samples histogram ===\n";
    std::normal_distribution<double> norm_wide{5.0, 2.0};
    std::uniform_int_distribution<int> norm_int_proxy{1, 100};
    std::vector<int> norm_int_samples(500);
    std::ranges::generate(norm_int_samples, [&]{
        return std::clamp(static_cast<int>(std::round(rng(norm_wide))), 1, 10);
    });
    print_histogram(norm_int_samples, 10, 25);

    std::cout << "\n=== std::sample (reservoir sampling) ===\n";
    const auto pop_view = std::views::iota(1, 21);
    const std::vector<int> population(pop_view.begin(), pop_view.end());
    std::vector<int> sampled(5);
    std::sample(population.begin(), population.end(),
                sampled.begin(), 5, rng.engine);
    print_vec(sampled, "5 from [1..20]");

    std::cout << "\n=== Weighted selection via discrete_distribution ===\n";
    constexpr std::array<std::string_view, 4> items{"common", "uncommon", "rare", "legendary"};
    std::discrete_distribution<int> rarity{{60, 25, 12, 3}};
    std::map<std::string_view, int> tally;
    for (int i = 0; i < 1000; ++i) ++tally[items[rng(rarity)]];
    for (const auto& [name, count] : tally)
        std::cout << std::format("  {:<10}: {:>4} ({:4.1f}%)\n",
                                 name, count, count / 10.0);

    std::cout << "\n=== Random string ===\n";
    constexpr std::string_view charset =
        "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::uniform_int_distribution<std::size_t> idx_dist{0, charset.size() - 1};
    std::string random_str(12, ' ');
    std::ranges::generate(random_str, [&]{ return charset[rng(idx_dist)]; });
    std::cout << std::format("Random string: {}\n", random_str);

    std::cout << "\n=== Reproducibility check ===\n";
    Rng rng_a{12345};
    Rng rng_b{12345};
    bool identical = true;
    for (int i = 0; i < 20; ++i)
        if (rng_a(dist_int) != rng_b(dist_int)) { identical = false; break; }
    std::cout << std::format("Two engines seeded identically produce same sequence: {}\n",
                             identical);
    assert(identical);

    return 0;
}
