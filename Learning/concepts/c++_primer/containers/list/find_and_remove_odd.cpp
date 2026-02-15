#include <forward_list>
#include <iostream>
#include <cstddef>
#include <algorithm>   // tiny addition
#include <numeric>     // tiny addition

// ------------------------------------------------
// FORWARD DECLARATION (SMALL ADDITION)
// ------------------------------------------------
std::size_t count_elements(const std::forward_list<int>& lst);

// ---- tiny extra forward declarations ----
int sum_elements(const std::forward_list<int>& lst);
int max_element_safe(const std::forward_list<int>& lst);
void print_divider();

// ------------------------------------------------
// ORIGINAL CODE (UNCHANGED)
// ------------------------------------------------

void find_and_remove_odd(std::forward_list<int>& lst) {
    auto prev = lst.before_begin();
    auto curr = lst.begin();

    while (curr != lst.end()) {
        if (*curr % 2) {                 // odd
            curr = lst.erase_after(prev);
        } else {
            prev = curr;
            ++curr;
        }
    }
}

void print_list(const std::forward_list<int>& lst) {
    for (auto elem : lst) {
        std::cout << '[' << elem << "]->";
    }
    std::cout << "\n";
}

int main() {
    // Test case 1: mixed numbers
    std::forward_list<int> fl1 = {1, 2, 3, 4, 5, 6, 7, 8};
    std::cout << "Original list 1: ";
    print_list(fl1);

    find_and_remove_odd(fl1);
    std::cout << "After removing odd numbers: ";
    print_list(fl1);
    std::cout << "Count: " << count_elements(fl1) << "\n";
    std::cout << (fl1.empty() ? "List is empty\n" : "List is not empty\n");

    // ---- tiny extra usage ----
    std::cout << "Sum: " << sum_elements(fl1) << "\n";
    std::cout << "Max: " << max_element_safe(fl1) << "\n";

    print_divider();

    // Test case 2: all odd numbers
    std::forward_list<int> fl2 = {1, 3, 5, 7, 9};
    std::cout << "\nOriginal list 2: ";
    print_list(fl2);

    find_and_remove_odd(fl2);
    std::cout << "After removing odd numbers: ";
    print_list(fl2);
    std::cout << "Count: " << count_elements(fl2) << "\n";
    std::cout << (fl2.empty() ? "List is empty\n" : "List is not empty\n");

    // ---- tiny extra usage ----
    std::cout << "Sum: " << sum_elements(fl2) << "\n";
    std::cout << "Max: " << max_element_safe(fl2) << "\n";

    print_divider();

    // Test case 3: all even numbers
    std::forward_list<int> fl3 = {2, 4, 6, 8, 10};
    std::cout << "\nOriginal list 3: ";
    print_list(fl3);

    find_and_remove_odd(fl3);
    std::cout << "After removing odd numbers: ";
    print_list(fl3);
    std::cout << "Count: " << count_elements(fl3) << "\n";
    std::cout << (fl3.empty() ? "List is empty\n" : "List is not empty\n");

    // ---- tiny extra usage ----
    std::cout << "Sum: " << sum_elements(fl3) << "\n";
    std::cout << "Max: " << max_element_safe(fl3) << "\n";

    print_divider();

    // Test case 4: empty list
    std::forward_list<int> fl4;
    std::cout << "\nOriginal list 4 (empty): ";
    print_list(fl4);

    find_and_remove_odd(fl4);
    std::cout << "After removing odd numbers: ";
    print_list(fl4);
    std::cout << "Count: " << count_elements(fl4) << "\n";
    std::cout << (fl4.empty() ? "List is empty\n" : "List is not empty\n");

    // ---- tiny extra usage ----
    std::cout << "Sum: " << sum_elements(fl4) << "\n";
    std::cout << "Max: " << max_element_safe(fl4) << "\n";

    return 0;
}

// ------------------------------------------------
// SMALL EXTRA CODE (MINIMAL ADDITION)
// ------------------------------------------------

// Count elements in a forward_list
std::size_t count_elements(const std::forward_list<int>& lst) {
    std::size_t count = 0;
    for (int v : lst) {
        (void)v;
        ++count;
    }
    return count;
}

// ---- VERY SMALL EXTRA HELPERS ----

// Sum elements
int sum_elements(const std::forward_list<int>& lst) {
    return std::accumulate(lst.begin(), lst.end(), 0);
}

// Safe max finder
int max_element_safe(const std::forward_list<int>& lst) {
    if (lst.begin() == lst.end()) return 0;
    return *std::max_element(lst.begin(), lst.end());
}

// Tiny divider
void print_divider() {
    std::cout << "----------------------\n";
}
