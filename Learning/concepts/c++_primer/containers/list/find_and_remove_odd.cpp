#include <forward_list>
#include <iostream>
#include <cstddef>

// ------------------------------------------------
// FORWARD DECLARATION (SMALL ADDITION)
// ------------------------------------------------
std::size_t count_elements(const std::forward_list<int>& lst);

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

    // Test case 2: all odd numbers
    std::forward_list<int> fl2 = {1, 3, 5, 7, 9};
    std::cout << "\nOriginal list 2: ";
    print_list(fl2);

    find_and_remove_odd(fl2);
    std::cout << "After removing odd numbers: ";
    print_list(fl2);

    // Test case 3: all even numbers
    std::forward_list<int> fl3 = {2, 4, 6, 8, 10};
    std::cout << "\nOriginal list 3: ";
    print_list(fl3);

    find_and_remove_odd(fl3);
    std::cout << "After removing odd numbers: ";
    print_list(fl3);

    // Test case 4: empty list
    std::forward_list<int> fl4;
    std::cout << "\nOriginal list 4 (empty): ";
    print_list(fl4);

    find_and_remove_odd(fl4);
    std::cout << "After removing odd numbers: ";
    print_list(fl4);

    // ---- SMALL ADDITION ----
    std::cout << "\nFinal element count (list 1): "
              << count_elements(fl1) << "\n";

    return 0;
}

// ------------------------------------------------
// SMALL EXTRA CODE (MINIMAL ADDITION)
// ------------------------------------------------

// Count elements in a forward_list
std::size_t count_elements(const std::forward_list<int>& lst) {
    std::size_t count = 0;
    for (int v : lst) {
        (void)v; // silence unused warning
        ++count;
    }
    return count;
}
 
