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

    // -----------------------------------

    return 0;
}
