// Sorting and Partitioning Exercise
// sort, stable_sort, nth_element, partition

#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    // sort
    std::vector<int> data = {5, 2, 8, 1, 9, 3};
    std::sort(data.begin(), data.end());
    
    std::cout << "Sorted: ";
    for (int v : data) std::cout << v << " ";
    std::cout << "\n";
    
    // stable_sort
    data = {5, 2, 8, 1, 9, 3};
    std::stable_sort(data.begin(), data.end());
    
    // nth_element
    data = {5, 2, 8, 1, 9, 3};
    std::nth_element(data.begin(), data.begin() + 2, data.end());
    std::cout << "Element at position 2: " << data[2] << "\n";
    
    // partition
    data = {5, 2, 8, 1, 9, 3};
    auto pivot = std::partition(data.begin(), data.end(), 
                               [](int x) { return x < 5; });
    std::cout << "After partition: ";
    for (int v : data) std::cout << v << " ";
    std::cout << "\n";

    // ---- additional small examples ----

    // check if sorted
    bool sorted = std::is_sorted(data.begin(), data.end());
    std::cout << "Is data sorted? " << (sorted ? "Yes" : "No") << "\n";

    // reverse the data
    std::reverse(data.begin(), data.end());
    std::cout << "Reversed data: ";
    for (int v : data) std::cout << v << " ";
    std::cout << "\n";

    // partition_point (works after partition)
    auto point = std::partition_point(data.begin(), data.end(),
                                     [](int x) { return x < 5; });
    std::cout << "Partition point index: "
              << std::distance(data.begin(), point) << "\n";

    // ---------------- NEW SMALL ADDITIONS ----------------

    // binary_search (requires sorted data)
    std::sort(data.begin(), data.end());
    bool found = std::binary_search(data.begin(), data.end(), 5);
    std::cout << "Binary search for 5: "
              << (found ? "Found" : "Not Found") << "\n";

    // lower_bound & upper_bound
    auto lb = std::lower_bound(data.begin(), data.end(), 5);
    auto ub = std::upper_bound(data.begin(), data.end(), 5);

    std::cout << "Lower bound index of 5: "
              << std::distance(data.begin(), lb) << "\n";

    std::cout << "Upper bound index of 5: "
              << std::distance(data.begin(), ub) << "\n";

    // equal_range
    auto range = std::equal_range(data.begin(), data.end(), 5);
    std::cout << "Equal range for 5: ["
              << std::distance(data.begin(), range.first) << ", "
              << std::distance(data.begin(), range.second) << ")\n";

    // partial_sort (top 3 smallest elements sorted)
    data = {5, 2, 8, 1, 9, 3};
    std::partial_sort(data.begin(), data.begin() + 3, data.end());

    std::cout << "Partial sort (first 3 smallest): ";
    for (int v : data) std::cout << v << " ";
    std::cout << "\n";

    // is_partitioned check
    data = {1, 2, 3, 6, 7, 8};
    bool is_part = std::is_partitioned(data.begin(), data.end(),
                                       [](int x) { return x < 5; });

    std::cout << "Is partitioned (<5)? "
              << (is_part ? "Yes" : "No") << "\n";

    // ----------------------------------------------------

    return 0;
}
