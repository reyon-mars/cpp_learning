#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

// ------------------------------------------------------
// FORWARD DECLARATION (required, no logic change)
// ------------------------------------------------------
void run_extra_vector_demo();

// ---- tiny extra forward declarations ----
int max_vector(const std::vector<int>& v);
int min_vector(const std::vector<int>& v);
double average_vector(const std::vector<int>& v);

// ---- extra tiny helpers ----
int count_even(const std::vector<int>& v);
int count_odd(const std::vector<int>& v);

// ---- NEW SMALL HELPERS ----
void print_divider();
void print_vector_info(const std::vector<int>& v);
bool is_sorted_vector(const std::vector<int>& v);

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

    // ---- NEW SMALL ADDITIONS ----
    print_divider();

    std::cout << "Vector info after reverse:\n";
    print_vector_info(vec);

    std::cout << "Is vector sorted? "
              << (is_sorted_vector(vec) ? "Yes" : "No") << '\n';

    // sort again
    std::sort(vec.begin(), vec.end());

    std::cout << "After sorting main vector: ";
    for (const auto num : vec) {
        std::cout << num << " ";
    }
    std::cout << '\n';

    std::cout << "Front element: " << vec.front() << '\n';
    std::cout << "Back element: " << vec.back() << '\n';

    print_divider();

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

// ---- VERY SMALL EXTRA HELPERS ----

// Max element
int max_vector(const std::vector<int>& v) {
    if (v.empty()) return 0;
    return *std::max_element(v.begin(), v.end());
}

// Min element
int min_vector(const std::vector<int>& v) {
    if (v.empty()) return 0;
    return *std::min_element(v.begin(), v.end());
}

// Average
double average_vector(const std::vector<int>& v) {
    if (v.empty()) return 0.0;
    return static_cast<double>(sum_vector(v)) / v.size();
}

// ---- EXTRA SMALL HELPERS ----

// Count even numbers
int count_even(const std::vector<int>& v) {
    return std::count_if(v.begin(), v.end(),
                         [](int x) { return x % 2 == 0; });
}

// Count odd numbers
int count_odd(const std::vector<int>& v) {
    return std::count_if(v.begin(), v.end(),
                         [](int x) { return x % 2 != 0; });
}

// ---- NEW SMALL HELPERS ----

// Divider
void print_divider() {
    std::cout << "-----------------------------\n";
}

// Print vector statistics
void print_vector_info(const std::vector<int>& v) {
    std::cout << "Size: " << v.size() << '\n';
    std::cout << "Sum: " << sum_vector(v) << '\n';
    std::cout << "Min: " << min_vector(v) << '\n';
    std::cout << "Max: " << max_vector(v) << '\n';
    std::cout << "Average: " << average_vector(v) << '\n';
    std::cout << "Even count: " << count_even(v) << '\n';
    std::cout << "Odd count: " << count_odd(v) << '\n';
}

// Check if vector is sorted
bool is_sorted_vector(const std::vector<int>& v) {
    return std::is_sorted(v.begin(), v.end());
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

    // ---- tiny extra usage ----
    std::cout << "Max = " << max_vector(extra) << '\n';
    std::cout << "Min = " << min_vector(extra) << '\n';
    std::cout << "Average = " << average_vector(extra) << '\n';

    // ---- extra tiny usage ----
    std::cout << "Even count = " << count_even(extra) << '\n';
    std::cout << "Odd count = " << count_odd(extra) << '\n';

    // Reverse demo
    std::reverse(extra.begin(), extra.end());
    std::cout << "Reversed: ";
    print_vector(extra);

    // Sort demo
    std::sort(extra.begin(), extra.end());
    std::cout << "Sorted: ";
    print_vector(extra);

    // ---- NEW SMALL TESTS ----

    print_divider();

    std::cout << "Extra vector info:\n";
    print_vector_info(extra);

    extra.push_back(10);
    extra.push_back(20);

    std::cout << "After push_back operations: ";
    print_vector(extra);

    extra.pop_back();

    std::cout << "After pop_back: ";
    print_vector(extra);

    std::cout << "Is sorted now? "
              << (is_sorted_vector(extra) ? "Yes" : "No") << '\n';
}
