// Associative Containers Exercise
// map, multimap, set, multiset

#include <iostream>
#include <map>
#include <set>

int main() {
    // std::map
    std::map<std::string, int> age_map;
    age_map["Alice"] = 30;
    age_map["Bob"] = 25;
    
    std::cout << "Alice's age: " << age_map["Alice"] << "\n";
    
    // std::set
    std::set<int> unique_values = {5, 2, 8, 2, 5, 1};
    std::cout << "Unique values: ";
    for (int v : unique_values) {
        std::cout << v << " ";
    }
    std::cout << "\n";

    // ---- Added examples ----

    // std::multimap
    std::multimap<std::string, int> scores;
    scores.insert({"Alice", 90});
    scores.insert({"Alice", 85});
    scores.insert({"Bob", 88});

    std::cout << "Multimap values:\n";
    for (const auto& pair : scores) {
        std::cout << pair.first << ": " << pair.second << "\n";
    }

    // std::multiset
    std::multiset<int> numbers = {5, 2, 8, 2, 5, 1};

    std::cout << "Multiset values: ";
    for (int n : numbers) {
        std::cout << n << " ";
    }
    std::cout << "\n";

    // ------------------------
    // EXTRA SMALL ADDITIONS
    // ------------------------

    // find in map
    auto it = age_map.find("Bob");
    if (it != age_map.end()) {
        std::cout << "Found Bob, age = " << it->second << "\n";
    }

    // check existence in set
    if (unique_values.count(5)) {
        std::cout << "5 exists in set\n";
    }

    // equal_range in multimap
    auto range = scores.equal_range("Alice");
    std::cout << "Scores for Alice: ";
    for (auto itr = range.first; itr != range.second; ++itr) {
        std::cout << itr->second << " ";
    }
    std::cout << "\n";

    // erase example
    unique_values.erase(2);
    std::cout << "After erasing 2: ";
    for (int v : unique_values) {
        std::cout << v << " ";
    }
    std::cout << "\n";

    // size checks
    std::cout << "Map size: " << age_map.size() << "\n";
    std::cout << "Set size: " << unique_values.size() << "\n";

    return 0;
}
