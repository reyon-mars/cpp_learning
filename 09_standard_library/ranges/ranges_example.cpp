// Ranges Exercise (C++20)
// Range-based algorithms and views

#include <iostream>
#include <vector>
#include <ranges>
#include <numeric>   // added
#include <algorithm> // added

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    // Range-based sort
    auto evens = vec | std::views::filter([](int x) { return x % 2 == 0; });
    
    std::cout << "Even numbers: ";
    for (int v : evens) {
        std::cout << v << " ";
    }
    std::cout << "\n";
    
    // Range-based transform
    auto doubled = vec | std::views::transform([](int x) { return x * 2; });
    
    std::cout << "Doubled: ";
    for (int v : doubled) {
        std::cout << v << " ";
    }
    std::cout << "\n";

    // ---- Additional small examples ----

    // take first 5 elements
    auto first_five = vec | std::views::take(5);
    std::cout << "First five: ";
    for (int v : first_five) {
        std::cout << v << " ";
    }
    std::cout << "\n";

    // drop first 5 elements
    auto after_five = vec | std::views::drop(5);
    std::cout << "After five: ";
    for (int v : after_five) {
        std::cout << v << " ";
    }
    std::cout << "\n";

    // reverse view
    auto reversed = vec | std::views::reverse;
    std::cout << "Reversed: ";
    for (int v : reversed) {
        std::cout << v << " ";
    }
    std::cout << "\n";

    // -----------------------------------

    // ===== SMALL ADDITIONS ONLY =====

    // Count elements using ranges
    auto count_all = std::ranges::distance(vec);
    std::cout << "Total elements: " << count_all << "\n";

    // Sum using accumulate
    int sum = std::accumulate(vec.begin(), vec.end(), 0);
    std::cout << "Sum of elements: " << sum << "\n";

    // Check if all elements are positive
    bool all_positive = std::ranges::all_of(vec, [](int x){ return x > 0; });
    std::cout << "All positive? " << (all_positive ? "Yes" : "No") << "\n";

    // Find first element > 5
    auto it = std::ranges::find_if(vec, [](int x){ return x > 5; });
    if (it != vec.end())
        std::cout << "First element > 5: " << *it << "\n";

    // Chained view (filter + transform)
    auto even_squared = vec
        | std::views::filter([](int x){ return x % 2 == 0; })
        | std::views::transform([](int x){ return x * x; });

    std::cout << "Even squared: ";
    for (int v : even_squared)
        std::cout << v << " ";
    std::cout << "\n";

    // ---- EXTRA SMALL ADDITIONS ----

    // Find max and min
    auto [min_it, max_it] = std::minmax_element(vec.begin(), vec.end());
    std::cout << "Min: " << *min_it << ", Max: " << *max_it << "\n";

    // Check if sorted
    bool sorted = std::ranges::is_sorted(vec);
    std::cout << "Is sorted? " << (sorted ? "Yes" : "No") << "\n";

    // Copy filtered results into a vector
    std::vector<int> even_vec;
    std::ranges::copy(evens, std::back_inserter(even_vec));

    std::cout << "Copied evens: ";
    for (int v : even_vec)
        std::cout << v << " ";
    std::cout << "\n";

    // Count even numbers
    auto even_count = std::ranges::count_if(vec, [](int x){ return x % 2 == 0; });
    std::cout << "Even count: " << even_count << "\n";

    // =================================

    return 0;
}
