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
    
    return 0;
}
