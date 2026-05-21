// Associative Containers Exercise
// map, multimap, set, multiset

#include <iostream>
#include <map>
#include <set>
#include <string>   // 🔹 NEW
#include <vector>   // 🔹 NEW
#include <algorithm> // 🔹 NEW

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

    // ---------------- NEW SMALL ADDITIONS ----------------

    // insert with hint (map)
    age_map.insert(age_map.begin(), {"Charlie", 28});
    std::cout << "Inserted Charlie\n";

    // emplace (more efficient insertion)
    age_map.emplace("David", 40);

    // iterate map (structured bindings)
    std::cout << "All map entries:\n";
    for (const auto& [name, age] : age_map) {
        std::cout << name << ": " << age << "\n";
    }

    // set lower_bound / upper_bound
    auto lb = unique_values.lower_bound(5);
    auto ub = unique_values.upper_bound(5);

    std::cout << "Lower bound of 5: "
              << (lb != unique_values.end() ? std::to_string(*lb) : "end") << "\n";

    std::cout << "Upper bound of 5: "
              << (ub != unique_values.end() ? std::to_string(*ub) : "end") << "\n";

    // multiset count
    std::cout << "Count of 5 in multiset: "
              << numbers.count(5) << "\n";

    // clear container demo
    numbers.clear();
    std::cout << "Multiset cleared, size: "
              << numbers.size() << "\n";

    // ----------------------------------------------------
    // 🔹 EXTRA ADDITIONS (ORIGINAL CODE UNCHANGED)
    // ----------------------------------------------------

    // map contains check (C++20)
    if (age_map.contains("Alice")) {
        std::cout << "Alice exists in map\n";
    }

    // update existing value
    age_map["Alice"] += 1;
    std::cout << "Updated Alice age: "
              << age_map["Alice"] << "\n";

    // copy map into another map
    std::map<std::string, int> copied_map = age_map;

    std::cout << "Copied map entries:\n";
    for (const auto& [name, age] : copied_map) {
        std::cout << name << ": " << age << "\n";
    }

    // swap sets
    std::set<int> other_set = {100, 200, 300};

    unique_values.swap(other_set);

    std::cout << "After swap, unique_values: ";
    for (int v : unique_values) {
        std::cout << v << " ";
    }
    std::cout << "\n";

    // restore values
    unique_values.swap(other_set);

    // multimap count
    std::cout << "Entries for Alice in multimap: "
              << scores.count("Alice") << "\n";

    // insert more duplicate values into multiset
    numbers.insert(10);
    numbers.insert(10);
    numbers.insert(20);

    std::cout << "Updated multiset values: ";
    for (int n : numbers) {
        std::cout << n << " ";
    }
    std::cout << "\n";

    // multiset equal_range
    auto num_range = numbers.equal_range(10);

    std::cout << "Values equal to 10: ";
    for (auto itr = num_range.first; itr != num_range.second; ++itr) {
        std::cout << *itr << " ";
    }
    std::cout << "\n";

    // vector to set conversion
    std::vector<int> values = {9, 8, 7, 7, 6, 5};
    std::set<int> converted(values.begin(), values.end());

    std::cout << "Converted set: ";
    for (int v : converted) {
        std::cout << v << " ";
    }
    std::cout << "\n";

    // check empty
    std::cout << "Is multiset empty? "
              << (numbers.empty() ? "Yes" : "No") << "\n";

    // erase by key in map
    age_map.erase("David");

    std::cout << "After erasing David:\n";
    for (const auto& [name, age] : age_map) {
        std::cout << name << ": " << age << "\n";
    }

    // ----------------------------------------------------

    return 0;
}
