// Unordered Containers Exercise
// unordered_map, unordered_set

#include <iostream>
#include <unordered_map>
#include <unordered_set>

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
    
    return 0;
}
