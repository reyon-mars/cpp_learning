// Algorithms Exercise
// Non-modifying algorithms (find, count, search, etc.)

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <iterator>   // 🔹 ADDED

int main() {

    std::vector<int> vec = {
        1, 2, 3, 4, 5, 3, 2, 1
    };

    // find
    auto it = std::find(
        vec.begin(),
        vec.end(),
        3
    );

    if (it != vec.end()) {

        std::cout
            << "Found 3 at index: "
            << std::distance(vec.begin(), it)
            << "\n";
    }

    // count
    int count = std::count(
        vec.begin(),
        vec.end(),
        3
    );

    std::cout
        << "Count of 3: "
        << count
        << "\n";

    // find_if
    auto even = std::find_if(
        vec.begin(),
        vec.end(),
        [](int x) {
            return x % 2 == 0;
        }
    );

    if (even != vec.end()) {

        std::cout
            << "First even: "
            << *even
            << "\n";
    }

    // search (find a subsequence)
    std::vector<int> pattern = {3, 2};

    auto pos = std::search(
        vec.begin(),
        vec.end(),
        pattern.begin(),
        pattern.end()
    );

    if (pos != vec.end()) {

        std::cout
            << "Subsequence {3,2} found at index: "
            << std::distance(vec.begin(), pos)
            << "\n";
    }

    // all_of
    bool allPositive = std::all_of(
        vec.begin(),
        vec.end(),
        [](int x) {
            return x > 0;
        }
    );

    std::cout
        << "All numbers are positive: "
        << (allPositive ? "Yes" : "No")
        << "\n";

    // any_of
    bool anyGreaterThanFour = std::any_of(
        vec.begin(),
        vec.end(),
        [](int x) {
            return x > 4;
        }
    );

    std::cout
        << "Any number greater than 4: "
        << (anyGreaterThanFour ? "Yes" : "No")
        << "\n";

    // none_of
    bool noneNegative = std::none_of(
        vec.begin(),
        vec.end(),
        [](int x) {
            return x < 0;
        }
    );

    std::cout
        << "No negative numbers: "
        << (noneNegative ? "Yes" : "No")
        << "\n";

    // ---------------- SMALL ADDITIONS ----------------

    // count_if
    int even_count = std::count_if(
        vec.begin(),
        vec.end(),
        [](int x) {
            return x % 2 == 0;
        }
    );

    std::cout
        << "Even numbers count: "
        << even_count
        << "\n";

    // find_if_not
    auto not_positive = std::find_if_not(
        vec.begin(),
        vec.end(),
        [](int x) {
            return x > 0;
        }
    );

    if (not_positive == vec.end()) {

        std::cout
            << "All elements are positive "
            << "(find_if_not check)\n";
    }

    // equal
    std::vector<int> vec2 = vec;

    bool is_equal = std::equal(
        vec.begin(),
        vec.end(),
        vec2.begin()
    );

    std::cout
        << "vec and vec2 are equal: "
        << (is_equal ? "Yes" : "No")
        << "\n";

    // mismatch
    vec2[0] = 99;

    auto mismatch_pair = std::mismatch(
        vec.begin(),
        vec.end(),
        vec2.begin()
    );

    if (mismatch_pair.first != vec.end()) {

        std::cout
            << "Mismatch at value: "
            << *mismatch_pair.first
            << " vs "
            << *mismatch_pair.second
            << "\n";
    }

    // accumulate
    int total = std::accumulate(
        vec.begin(),
        vec.end(),
        0
    );

    std::cout
        << "Sum of elements: "
        << total
        << "\n";

    // 🔹 NEW: find_end
    auto last_pos = std::find_end(
        vec.begin(),
        vec.end(),
        pattern.begin(),
        pattern.end()
    );

    if (last_pos != vec.end()) {

        std::cout
            << "Last occurrence of {3,2} at index: "
            << std::distance(vec.begin(), last_pos)
            << "\n";
    }

    // 🔹 NEW: adjacent_find
    auto adj = std::adjacent_find(
        vec.begin(),
        vec.end()
    );

    if (adj != vec.end()) {

        std::cout
            << "First adjacent duplicate: "
            << *adj
            << "\n";

    } else {

        std::cout
            << "No adjacent duplicates found\n";
    }

    // 🔹 NEW: search_n
    auto repeated = std::search_n(
        vec.begin(),
        vec.end(),
        2,
        3
    );

    if (repeated != vec.end()) {

        std::cout
            << "Two consecutive 3s found at index: "
            << std::distance(vec.begin(), repeated)
            << "\n";

    } else {

        std::cout
            << "No consecutive 3s found\n";
    }

    // 🔹 NEW: min and max elements
    auto min_it = std::min_element(
        vec.begin(),
        vec.end()
    );

    auto max_it = std::max_element(
        vec.begin(),
        vec.end()
    );

    std::cout
        << "Min element: "
        << *min_it
        << "\n";

    std::cout
        << "Max element: "
        << *max_it
        << "\n";

    // ---------------- EXTRA ADDITIONS ----------------

    // 🔹 NEW: binary_search
    std::vector<int> sorted_vec = vec;

    std::sort(
        sorted_vec.begin(),
        sorted_vec.end()
    );

    bool found = std::binary_search(
        sorted_vec.begin(),
        sorted_vec.end(),
        4
    );

    std::cout
        << "Binary search for 4: "
        << (found ? "Found" : "Not Found")
        << "\n";

    // 🔹 NEW: lower_bound
    auto lb = std::lower_bound(
        sorted_vec.begin(),
        sorted_vec.end(),
        3
    );

    std::cout
        << "Lower bound of 3 at index: "
        << std::distance(sorted_vec.begin(), lb)
        << "\n";

    // 🔹 NEW: upper_bound
    auto ub = std::upper_bound(
        sorted_vec.begin(),
        sorted_vec.end(),
        3
    );

    std::cout
        << "Upper bound of 3 at index: "
        << std::distance(sorted_vec.begin(), ub)
        << "\n";

    // 🔹 NEW: equal_range
    auto range = std::equal_range(
        sorted_vec.begin(),
        sorted_vec.end(),
        2
    );

    std::cout
        << "Equal range for 2: "
        << std::distance(sorted_vec.begin(),
                         range.first)
        << " to "
        << std::distance(sorted_vec.begin(),
                         range.second)
        << "\n";

    // 🔹 NEW: is_sorted
    std::cout
        << "sorted_vec is sorted: "
        << (std::is_sorted(
                sorted_vec.begin(),
                sorted_vec.end()
            ) ? "Yes" : "No")
        << "\n";

    // 🔹 NEW: partial_sum
    std::vector<int> partial(vec.size());

    std::partial_sum(
        vec.begin(),
        vec.end(),
        partial.begin()
    );

    std::cout
        << "Partial sums: ";

    for (int n : partial) {
        std::cout << n << " ";
    }

    std::cout << "\n";

    return 0;
}
