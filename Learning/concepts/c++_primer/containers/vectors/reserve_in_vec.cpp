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
// EXTRA CODE (SMALL & APPENDED ONLY)
// ======================================================

// Print vector elements
void print_vector(const std::vector<int>& v) {
    std::cout << "[Extra] Vector: ";
    for (int x : v)
        std::cout << x << " ";
    std::cout << '\n';
}

// Generate even numbers
std::vector<int> generate_even_vector(int n) {
    std::vector<int> v;
    v.reserve(n);

    for (int i = 0; i < n; i++)
        v.push_back(i * 2);

    return v;
}

// Sum all elements
int sum_vector(const std::vector<int>& v) {
    return std::accumulate(v.begin(), v.end(), 0);
}

// ---- VERY SMALL EXTRA HELPERS ----

// Find max element safely
int max_element_safe(const std::vector<int>& v) {
    return v.empty() ? 0 : *std::max_element(v.begin(), v.end());
}

// Find min element safely
int min_element_safe(const std::vector<int>& v) {
    return v.empty() ? 0 : *std::min_element(v.begin(), v.end());
}

// Average of vector
double average_vector(const std::vector<int>& v) {
    if (v.empty()) return 0.0;
    return static_cast<double>(sum_vector(v)) / v.size();
}

// Check if vector is empty
bool is_empty(const std::vector<int>& v) {
    return v.empty();
}

// Check if value exists
bool contains_value(const std::vector<int>& v, int value) {
    return std::find(v.begin(), v.end(), value) != v.end();
}
// ---------------------------------

void run_extra_vector_demo() {
    std::cout << "\n--- Extra Vector Demo ---\n";

    auto evenVec = generate_even_vector(10);
    print_vector(evenVec);

    std::reverse(evenVec.begin(), evenVec.end());
    print_vector(evenVec);

    std::cout << "[Extra] Sum = " << sum_vector(evenVec) << '\n';
    std::cout << "[Extra] Max = " << max_element_safe(evenVec) << '\n';
    std::cout << "[Extra] Min = " << min_element_safe(evenVec) << '\n';
    std::cout << "[Extra] Avg = " << average_vector(evenVec) << '\n';

    std::cout << "[Extra] Contains 8? "
              << (contains_value(evenVec, 8) ? "Yes" : "No") << '\n';

    std::cout << "[Extra] Size = " << evenVec.size()
              << ", Capacity = " << evenVec.capacity() << '\n';

    std::cout << "[Extra] Is empty? "
              << (is_empty(evenVec) ? "Yes" : "No") << '\n';
}
 
