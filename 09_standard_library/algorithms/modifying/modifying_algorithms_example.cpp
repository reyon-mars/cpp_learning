// Algorithms Exercise
// Non-modifying algorithms (find, count, search, etc.)

#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5, 3, 2, 1};
    
    // find
    auto it = std::find(vec.begin(), vec.end(), 3);
    if (it != vec.end()) {
        std::cout << "Found 3 at index: " << std::distance(vec.begin(), it) << "\n";
    }
    
    // count
    int count = std::count(vec.begin(), vec.end(), 3);
    std::cout << "Count of 3: " << count << "\n";
    
    // find_if
    auto even = std::find_if(vec.begin(), vec.end(), 
                             [](int x) { return x % 2 == 0; });
    if (even != vec.end()) {
        std::cout << "First even: " << *even << "\n";
    }
    
    return 0;
}
