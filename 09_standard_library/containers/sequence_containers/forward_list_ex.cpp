#include <forward_list>
#include <iostream>
#include <algorithm>   // 🔹 NEW

// ======================================================
// ORIGINAL CODE (LOGIC UNCHANGED)
// ======================================================

int main(void) {

    std::forward_list<int> f_lst = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    auto prev = f_lst.before_begin();
    auto curr = f_lst.begin();

    while (curr != f_lst.end()) {
        if (*curr % 2) {                 // odd → remove
            curr = f_lst.erase_after(prev);
        } else {                         // even → keep
            prev = curr;
            ++curr;
        }
    }

    // Print remaining elements (even numbers)
    std::cout << "Even elements: ";
    for (int elem : f_lst) {
        std::cout << elem << " ";
    }
    std::cout << "\n";

    // ==================================================
    // SMALL ADDITION ONLY
    // ==================================================

    int count = 0;
    int sum = 0;
    int last = 0;

    for (int v : f_lst) {
        ++count;
        sum += v;
        last = v;
    }

    std::cout << "Count: " << count << "\n";
    std::cout << "Sum: " << sum << "\n";

    if (!f_lst.empty())
        std::cout << "Last element: " << last << "\n";
    else
        std::cout << "List is empty\n";

    // ---- additional small additions ----

    // Check if all elements are even (should be true)
    bool all_even = true;
    for (int v : f_lst) {
        if (v % 2 != 0) {
            all_even = false;
            break;
        }
    }
    std::cout << "All elements even? "
              << (all_even ? "Yes" : "No") << "\n";

    // Find first element greater than 4
    for (int v : f_lst) {
        if (v > 4) {
            std::cout << "First element > 4: " << v << "\n";
            break;
        }
    }

    // Print size info message
    std::cout << "List is "
              << (count == 0 ? "empty" : "not empty")
              << "\n";

    // ---------------- EXTRA SMALL ADDITIONS ----------------

    // Find minimum and maximum
    int min_val = 0, max_val = 0;
    if (!f_lst.empty()) {
        auto it = f_lst.begin();
        min_val = max_val = *it;
        for (; it != f_lst.end(); ++it) {
            if (*it < min_val) min_val = *it;
            if (*it > max_val) max_val = *it;
        }
        std::cout << "Min: " << min_val << "\n";
        std::cout << "Max: " << max_val << "\n";
    }

    // Average
    double avg = (count == 0) ? 0.0 : static_cast<double>(sum) / count;
    std::cout << "Average: " << avg << "\n";

    // Check if value exists
    int target = 6;
    bool found = false;
    for (int v : f_lst) {
        if (v == target) {
            found = true;
            break;
        }
    }
    std::cout << "Contains 6? "
              << (found ? "Yes" : "No") << "\n";

    // Reverse list
    f_lst.reverse();
    std::cout << "Reversed list: ";
    for (int v : f_lst) {
        std::cout << v << " ";
    }
    std::cout << "\n";

    // Sort list (safe even if already sorted)
    f_lst.sort();
    std::cout << "Sorted list: ";
    for (int v : f_lst) {
        std::cout << v << " ";
    }
    std::cout << "\n";

    // ---------------- NEW SMALL ADDITIONS ----------------

    // Count even numbers
    int even_count = 0;
    for (int v : f_lst) {
        if (v % 2 == 0)
            ++even_count;
    }

    std::cout << "Even count: "
              << even_count << "\n";

    // Product of all elements
    int product = 1;

    if (!f_lst.empty()) {
        for (int v : f_lst) {
            product *= v;
        }
        std::cout << "Product: "
                  << product << "\n";
    }

    // Push front
    f_lst.push_front(100);
    std::cout << "After push_front: ";
    for (int v : f_lst) {
        std::cout << v << " ";
    }
    std::cout << "\n";

    // Pop front
    f_lst.pop_front();
    std::cout << "After pop_front: ";
    for (int v : f_lst) {
        std::cout << v << " ";
    }
    std::cout << "\n";

    // Remove a specific value
    f_lst.remove(4);
    std::cout << "After removing 4: ";
    for (int v : f_lst) {
        std::cout << v << " ";
    }
    std::cout << "\n";

    // Unique example
    f_lst.push_front(2);
    f_lst.push_front(2);
    f_lst.unique();

    std::cout << "After unique(): ";
    for (int v : f_lst) {
        std::cout << v << " ";
    }
    std::cout << "\n";

    // Check if sorted
    std::cout << "Is sorted? "
              << (std::is_sorted(f_lst.begin(), f_lst.end())
                  ? "Yes" : "No")
              << "\n";

    // Resize example
    f_lst.resize(4);
    std::cout << "After resize(4): ";
    for (int v : f_lst) {
        std::cout << v << " ";
    }
    std::cout << "\n";

    // Clear list
    f_lst.clear();
    std::cout << "After clear, empty? "
              << (f_lst.empty() ? "Yes" : "No")
              << "\n";

    // ------------------------------------------------------

    return 0;
}
