// Unordered Containers Exercise
// unordered_map, unordered_set

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>   // tiny addition
#include <vector>      // 🔹 NEW
#include <numeric>     // 🔹 NEW

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

    // ---------------- NEW SMALL ADDITIONS ----------------

    // emplace example
    counts.emplace("grape", 4);

    std::cout << "After emplace:\n";
    for (const auto& [key, val] : counts) {
        std::cout << key << ": " << val << "\n";
    }

    // Count total values in map
    int total = 0;
    for (const auto& [key, val] : counts) {
        total += val;
    }

    std::cout << "Total stored counts: "
              << total << "\n";

    // Rehash demo
    counts.rehash(20);

    std::cout << "Bucket count after rehash: "
              << counts.bucket_count() << "\n";

    // Swap unordered sets
    std::unordered_set<int> another_set = {50, 60};

    unique_nums.swap(another_set);

    std::cout << "unique_nums after swap: ";
    for (int n : unique_nums) {
        std::cout << n << " ";
    }
    std::cout << "\n";

    // Find specific element in set
    auto set_it = unique_nums.find(50);

    if (set_it != unique_nums.end()) {
        std::cout << "Found value in set: "
                  << *set_it << "\n";
    }

    // Vector to unordered_set conversion
    std::vector<int> values = {7, 7, 8, 9, 9};

    std::unordered_set<int> converted(values.begin(), values.end());

    std::cout << "Converted set size: "
              << converted.size() << "\n";

    // ----------------------------------------------------

    return 0;
}
