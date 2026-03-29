// Algorithms Exercise
// Non-modifying algorithms (find, count, search, etc.)

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

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

    // search (find a subsequence)
    std::vector<int> pattern = {3, 2};
    auto pos = std::search(vec.begin(), vec.end(), pattern.begin(), pattern.end());
    if (pos != vec.end()) {
        std::cout << "Subsequence {3,2} found at index: " 
                  << std::distance(vec.begin(), pos) << "\n";
    }

    // all_of
    bool allPositive = std::all_of(vec.begin(), vec.end(), [](int x) { return x > 0; });
    std::cout << "All numbers are positive: " << (allPositive ? "Yes" : "No") << "\n";

    // any_of
    bool anyGreaterThanFour = std::any_of(vec.begin(), vec.end(), [](int x) { return x > 4; });
    std::cout << "Any number greater than 4: " << (anyGreaterThanFour ? "Yes" : "No") << "\n";

    // none_of
    bool noneNegative = std::none_of(vec.begin(), vec.end(), [](int x) { return x < 0; });
    std::cout << "No negative numbers: " << (noneNegative ? "Yes" : "No") << "\n";

    // ---------------- SMALL ADDITIONS ----------------

    // count_if
    int even_count = std::count_if(vec.begin(), vec.end(),
                                  [](int x) { return x % 2 == 0; });
    std::cout << "Even numbers count: " << even_count << "\n";

    // find_if_not
    auto not_positive = std::find_if_not(vec.begin(), vec.end(),
                                        [](int x) { return x > 0; });
    if (not_positive == vec.end()) {
        std::cout << "All elements are positive (find_if_not check)\n";
    }

    // equal (compare with another vector)
    std::vector<int> vec2 = vec;
    bool is_equal = std::equal(vec.begin(), vec.end(), vec2.begin());
    std::cout << "vec and vec2 are equal: " << (is_equal ? "Yes" : "No") << "\n";

    // mismatch
    vec2[0] = 99;
    auto mismatch_pair = std::mismatch(vec.begin(), vec.end(), vec2.begin());
    if (mismatch_pair.first != vec.end()) {
        std::cout << "Mismatch at value: "
                  << *mismatch_pair.first << " vs "
                  << *mismatch_pair.second << "\n";
    }

    // accumulate (sum)
    int total = std::accumulate(vec.begin(), vec.end(), 0);
    std::cout << "Sum of elements: " << total << "\n";

    return 0;
}
