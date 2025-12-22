#include <iostream>
#include <vector>
#include <algorithm>

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

    // ---- call extra demo explicitly ----
    run_extra_vector_demo();

    return 0;
}



// ======================================================
// EXTRA CODE ADDED BELOW (append only)
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

void run_extra_vector_demo() {
    std::cout << "\n--- Extra Vector Demo ---\n";

    auto evenVec = generate_even_vector(10);
    print_vector(evenVec);

    std::reverse(evenVec.begin(), evenVec.end());
    std::cout << "[Extra] Reversed even vector: ";
    print_vector(evenVec);

    std::cout << "[Extra] Size = " << evenVec.size()
              << ", Capacity = " << evenVec.capacity() << '\n';
}
