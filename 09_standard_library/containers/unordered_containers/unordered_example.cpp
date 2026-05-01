// Unordered Containers Exercise
// unordered_map, unordered_set

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>   // tiny addition

int main() {
    std::unordered_map<std::string, int> counts;
    counts["apple"]++;
    counts["banana"]++;
    counts["apple"]++;
    
    for (const auto& [key, val] : counts) {
        std::cout << key << ": " << val << "\n";
    }
    
    std::unordered_set<int> unique_nums = {1, 2, 3, 2, 1};
    std::cout << "Unique count: " << unique_nums.size() << "\n";

    // ---- small additions ----

    // Check if key exists
    std::cout << "Contains 'apple'? "
              << (counts.find("apple") != counts.end() ? "Yes" : "No") << "\n";

    // Access with at()
    if (counts.find("banana") != counts.end()) {
        std::cout << "Banana count (using at): "
                  << counts.at("banana") << "\n";
    }

    // Iterate only keys
    std::cout << "Keys: ";
    for (const auto& [key, _] : counts) {
        std::cout << key << " ";
    }
    std::cout << "\n";

    // unordered_set contains check
    std::cout << "Set contains 2? "
              << (unique_nums.count(2) ? "Yes" : "No") << "\n";

    // Insert new value
    unique_nums.insert(10);

    // Print set
    std::cout << "Set elements: ";
    for (int n : unique_nums) {
        std::cout << n << " ";
    }
    std::cout << "\n";

    // Erase element
    unique_nums.erase(1);
    std::cout << "After erasing 1, size: "
              << unique_nums.size() << "\n";

    // ------------------------
    // EXTRA SMALL ADDITIONS
    // ------------------------

    // Use iterator (avoid double lookup)
    auto it = counts.find("apple");
    if (it != counts.end()) {
        std::cout << "Iterator access -> apple: "
                  << it->second << "\n";
    }

    // Bucket information
    std::cout << "Bucket count: "
              << counts.bucket_count() << "\n";

    // Load factor
    std::cout << "Load factor: "
              << counts.load_factor() << "\n";

    // Insert without overwrite
    counts.insert({"orange", 1});
    std::cout << "After inserting orange:\n";
    for (const auto& [key, val] : counts) {
        std::cout << key << ": " << val << "\n";
    }

    // Clear set demo
    std::unordered_set<int> temp_set = unique_nums;
    temp_set.clear();
    std::cout << "Temp set cleared, size: "
              << temp_set.size() << "\n";

    return 0;
}
