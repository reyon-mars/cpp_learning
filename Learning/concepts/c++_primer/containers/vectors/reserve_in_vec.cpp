#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

// ------------------------------------------------------
// FORWARD DECLARATION (required, no logic change)
// ------------------------------------------------------
void run_extra_vector_demo();

// ======================================================
// ORIGINAL CODE (UNCHANGED LOGIC)
// ======================================================

int main(void) {
    std::vector<int> vec;
    vec.reserve(28);

    for (int i = 0; i < 28; i++) {
        vec.push_back(i);
    }

    std::cout << "Capacity " << vec.capacity()
              << " | Size " << vec.size() << std::endl;

    std::reverse(vec.begin(), vec.end());

    for (const auto num : vec) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    // ---- small extra call ----
    run_extra_vector_demo();

    return 0;
}

// ======================================================
// SMALL EXTRA CODE (ADDED ONLY)
// ======================================================

// Print vector elements
void print_vector(const std::vector<int>& v) {
    for (int x : v)
        std::cout << x << " ";
    std::cout << '\n';
}

// Sum of vector
int sum_vector(const std::vector<int>& v) {
    return std::accumulate(v.begin(), v.end(), 0);
}

// Check if value exists
bool contains_value(const std::vector<int>& v, int value) {
    return std::find(v.begin(), v.end(), value) != v.end();
}

// ======================================================
// EXTRA DEMO (VERY SMALL)
// ======================================================

void run_extra_vector_demo() {
    std::cout << "\n--- Extra Vector Demo ---\n";

    std::vector<int> extra = {1, 2, 3, 4, 5};
    print_vector(extra);

    std::cout << "Sum = " << sum_vector(extra) << '\n';

    std::cout << "Contains 3? "
              << (contains_value(extra, 3) ? "Yes" : "No") << '\n';
}
