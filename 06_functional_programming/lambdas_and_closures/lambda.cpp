#include <iostream>
#include <vector>
#include <numeric>
#include <utility>
#include <memory>
#include <algorithm>

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

    // IILE (Immediately Invoked Lambda Expression)
    int x = [vec](int a, int b) {
        return std::accumulate(vec.begin(), vec.end(), 0) * a * b;
    }(1, 1);

    std::cout << "IILE result: " << x << '\n';

    auto fn = sum_vector(vec);
    std::cout << "First sum_vector call: " << fn() << '\n';
    std::cout << "Second sum_vector call: " << fn() << '\n';

    // ---------------- SMALL ADDITIONS ----------------

    // Simple lambda
    auto square = [](int n) { return n * n; };
    std::cout << "Square of 6 = " << square(6) << '\n';

    // Predicate lambda
    auto is_even = [](int n) { return n % 2 == 0; };
    std::cout << "Is 10 even? " << (is_even(10) ? "Yes" : "No") << '\n';

    // Transform with lambda
    std::vector<int> doubled(vec.size());
    std::transform(vec.begin(), vec.end(), doubled.begin(),
                   [](int n) { return n * 2; });

    std::cout << "Doubled vector: ";
    for (int n : doubled)
        std::cout << n << " ";
    std::cout << '\n';

    // Count with lambda
    int even_count = std::count_if(doubled.begin(), doubled.end(), is_even);
    std::cout << "Even numbers in doubled vector: "
              << even_count << '\n';

    // Capture by reference demo
    auto print_size = [&]() {
        std::cout << "Original vector size: " << vec.size() << '\n';
    };
    print_size();

    // Mutable capture demo
    auto counter = [count = 0]() mutable {
        return ++count;
    };
    std::cout << "Counter calls: "
              << counter() << ", "
              << counter() << ", "
              << counter() << '\n';

    // Move capture (unique_ptr)
    auto ptr_lambda = [ptr = std::make_unique<int>(42)]() {
        return *ptr;
    };
    std::cout << "Moved unique_ptr value: "
              << ptr_lambda() << '\n';

    // Generic lambda (C++14+)
    auto add = [](auto a, auto b) {
        return a + b;
    };
    std::cout << "Generic add: "
              << add(3, 4.5) << '\n';

    // Lambda returning lambda
    auto make_multiplier = [](int factor) {
        return [factor](int x) {
            return x * factor;
        };
    };
    auto triple = make_multiplier(3);
    std::cout << "Triple of 7: "
              << triple(7) << '\n';

    // for_each accumulation
    int total = 0;
    std::for_each(vec.begin(), vec.end(),
                  [&](int n) { total += n; });
    std::cout << "Sum using for_each: "
              << total << '\n';

    // 🔹 NEW: remove_if + erase (filter odds)
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

    // 🔹 NEW: sort with lambda (descending)
    std::vector<int> sorted = vec;
    std::sort(sorted.begin(), sorted.end(),
              [](int a, int b) { return a > b; });
    std::cout << "Sorted descending: ";
    for (int n : sorted)
        std::cout << n << " ";
    std::cout << '\n';

    // 🔹 NEW: all_of / any_of demo
    bool all_even = std::all_of(vec.begin(), vec.end(), is_even);
    bool any_even = std::any_of(vec.begin(), vec.end(), is_even);
    std::cout << "All even? " << (all_even ? "Yes" : "No") << '\n';
    std::cout << "Any even? " << (any_even ? "Yes" : "No") << '\n';

    // 🔹 NEW: accumulate with lambda (custom logic)
    int weighted_sum = std::accumulate(vec.begin(), vec.end(), 0,
        [](int acc, int val) {
            return acc + val * 2;
        });
    std::cout << "Weighted sum (x2): "
              << weighted_sum << '\n';

    std::cout << "Program finished successfully.\n";

    return 0;
}
