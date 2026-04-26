#include <iostream>
#include <vector>
#include <numeric>
#include <utility>
#include <memory>
#include <algorithm>
#include <functional>   // 🔹 ADDED
#include <chrono>       // 🔹 ADDED

// ---------------- ORIGINAL FUNCTION ----------------
auto sum_vector(std::vector<int> vec) {
    return [v = std::move(vec)]() mutable {
        int sum = std::accumulate(v.begin(), v.end(), 0);
        v.clear();
        return sum;
    };
}

// ---------------- MAIN ----------------
int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5};

    int x = [vec](int a, int b) {
        return std::accumulate(vec.begin(), vec.end(), 0) * a * b;
    }(1, 1);

    std::cout << "IILE result: " << x << '\n';

    auto fn = sum_vector(vec);
    std::cout << "First sum_vector call: " << fn() << '\n';
    std::cout << "Second sum_vector call: " << fn() << '\n';

    // ---------------- EXISTING CODE ----------------
    auto square = [](int n) { return n * n; };
    std::cout << "Square of 6 = " << square(6) << '\n';

    auto is_even = [](int n) { return n % 2 == 0; };
    std::cout << "Is 10 even? " << (is_even(10) ? "Yes" : "No") << '\n';

    std::vector<int> doubled(vec.size());
    std::transform(vec.begin(), vec.end(), doubled.begin(),
                   [](int n) { return n * 2; });

    std::cout << "Doubled vector: ";
    for (int n : doubled)
        std::cout << n << " ";
    std::cout << '\n';

    int even_count = std::count_if(doubled.begin(), doubled.end(), is_even);
    std::cout << "Even numbers in doubled vector: "
              << even_count << '\n';

    auto print_size = [&]() {
        std::cout << "Original vector size: " << vec.size() << '\n';
    };
    print_size();

    auto counter = [count = 0]() mutable {
        return ++count;
    };
    std::cout << "Counter calls: "
              << counter() << ", "
              << counter() << ", "
              << counter() << '\n';

    auto ptr_lambda = [ptr = std::make_unique<int>(42)]() {
        return *ptr;
    };
    std::cout << "Moved unique_ptr value: "
              << ptr_lambda() << '\n';

    auto add = [](auto a, auto b) {
        return a + b;
    };
    std::cout << "Generic add: "
              << add(3, 4.5) << '\n';

    auto make_multiplier = [](int factor) {
        return [factor](int x) {
            return x * factor;
        };
    };
    auto triple = make_multiplier(3);
    std::cout << "Triple of 7: "
              << triple(7) << '\n';

    int total = 0;
    std::for_each(vec.begin(), vec.end(),
                  [&](int n) { total += n; });
    std::cout << "Sum using for_each: "
              << total << '\n';

    std::vector<int> filtered = vec;
    filtered.erase(
        std::remove_if(filtered.begin(), filtered.end(),
                       [](int n) { return n % 2 != 0; }),
        filtered.end()
    );

    std::cout << "Filtered (only even): ";
    for (int n : filtered)
        std::cout << n << " ";
    std::cout << '\n';

    std::vector<int> sorted = vec;
    std::sort(sorted.begin(), sorted.end(),
              [](int a, int b) { return a > b; });

    std::cout << "Sorted descending: ";
    for (int n : sorted)
        std::cout << n << " ";
    std::cout << '\n';

    bool all_even = std::all_of(vec.begin(), vec.end(), is_even);
    bool any_even = std::any_of(vec.begin(), vec.end(), is_even);

    std::cout << "All even? " << (all_even ? "Yes" : "No") << '\n';
    std::cout << "Any even? " << (any_even ? "Yes" : "No") << '\n';

    int weighted_sum = std::accumulate(vec.begin(), vec.end(), 0,
        [](int acc, int val) {
            return acc + val * 2;
        });

    std::cout << "Weighted sum (x2): "
              << weighted_sum << '\n';

    // ======================================================
    // 🔥 NEW ADDITIONS
    // ======================================================

    std::cout << "\n--- Advanced Lambda Features ---\n";

    // 🔹 Recursive lambda (factorial)
    std::function<int(int)> fact = [&](int n) {
        return (n <= 1) ? 1 : n * fact(n - 1);
    };
    std::cout << "Factorial(5): " << fact(5) << '\n';

    // 🔹 Init capture + reference mix
    int base = 10;
    auto complex_capture = [b = base, &vec]() {
        return b + vec.size();
    };
    std::cout << "Init + ref capture: "
              << complex_capture() << '\n';

    // 🔹 Lambda as custom comparator (pair sorting)
    std::vector<std::pair<int,int>> pairs = {{1,2},{3,1},{2,5}};
    std::sort(pairs.begin(), pairs.end(),
        [](auto& a, auto& b) {
            return a.second < b.second;
        });

    std::cout << "Sorted pairs by second: ";
    for (auto& p : pairs)
        std::cout << "(" << p.first << "," << p.second << ") ";
    std::cout << '\n';

    // 🔹 Deferred execution (lazy lambda)
    auto lazy = [=]() {
        return std::accumulate(vec.begin(), vec.end(), 0);
    };
    std::cout << "Lazy execution result: "
              << lazy() << '\n';

    // 🔹 Timing wrapper using lambda
    auto measure = [](auto func) {
        auto start = std::chrono::steady_clock::now();
        func();
        auto end = std::chrono::steady_clock::now();

        std::cout << "Execution time: "
                  << std::chrono::duration_cast<std::chrono::microseconds>(
                         end - start).count()
                  << " us\n";
    };

    measure([&]() {
        std::accumulate(vec.begin(), vec.end(), 0);
    });

    // ======================================================

    std::cout << "Program finished successfully.\n";

    return 0;
}
