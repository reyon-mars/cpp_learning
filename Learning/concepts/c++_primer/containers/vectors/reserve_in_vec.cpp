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

void print_vector(const std::vector<int>& v) {
    std::cout << "[Extra] Vector: ";
    for (int x : v)
        std::cout << x << " ";
    std::cout << '\n';
}

std::vector<int> generate_even_vector(int n) {
    std::vector<int> v;
    v.reserve(n);

    for (int i = 0; i < n; i++)
        v.push_back(i * 2);

    return v;
}

// ---- very small added helper ----
int sum_vector(const std::vector<int>& v) {
    return std::accumulate(v.begin(), v.end(), 0);
}

void run_extra_vector_demo() {
    std::cout << "\n--- Extra Vector Demo ---\n";

    auto evenVec = generate_even_vector(10);
    print_vector(evenVec);

    std::reverse(evenVec.begin(), evenVec.end());
    print_vector(evenVec);

    // ---- tiny added usage ----
    std::cout << "[Extra] Sum = " << sum_vector(evenVec) << '\n';

    std::cout << "[Extra] Size = " << evenVec.size()
              << ", Capacity = " << evenVec.capacity() << '\n';
}
 
