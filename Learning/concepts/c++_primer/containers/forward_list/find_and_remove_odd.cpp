#include <forward_list>
#include <iostream>

// ======================================================
// ORIGINAL CODE (LOGIC UNCHANGED)
// ======================================================

void find_and_remove_odd(std::forward_list<int>& lst) {
    auto prev = lst.before_begin();
    auto curr = lst.begin();

    while (curr != lst.end()) {
        if (*curr % 2 != 0) {              // odd number
            curr = lst.erase_after(prev); // erase and advance safely
        } else {
            prev = curr;
            ++curr;
        }
    }
}

void print_list(const std::forward_list<int>& lst) {
    for (auto elem : lst) {
        std::cout << "[" << elem << "]->";
    }
    std::cout << "\n";
}

// ======================================================
// SMALL ADDITION (HELPER FUNCTIONS ONLY)
// ======================================================

int count_elements(const std::forward_list<int>& lst) {
    int count = 0;
    for (int _ : lst) {
        (void)_;
        ++count;
    }
    return count;
}

bool is_empty(const std::forward_list<int>& lst) {
    return lst.begin() == lst.end();
}

int sum_elements(const std::forward_list<int>& lst) {
    int sum = 0;
    for (int v : lst)
        sum += v;
    return sum;
}

// ---- very small added helpers ----

// Return last element safely (assumes non-empty)
int last_element(const std::forward_list<int>& lst) {
    int last = 0;
    for (int v : lst)
        last = v;
    return last;
}

// Check if all elements are even
bool all_even(const std::forward_list<int>& lst) {
    for (int v : lst)
        if (v % 2 != 0)
            return false;
    return true;
}

// Print divider
void print_divider() {
    std::cout << "-----------------------------\n";
}
// --------------------------------

// ======================================================
// MAIN
// ======================================================

int main() {
    // Test case 1: mixed numbers
    std::forward_list<int> fl1 = {1, 2, 3, 4, 5, 6, 7, 8};
    std::cout << "Original list 1: ";
    print_list(fl1);

    find_and_remove_odd(fl1);
    std::cout << "After removing odd numbers: ";
    print_list(fl1);

    std::cout << "Count: " << count_elements(fl1) << "\n";
    std::cout << "Sum: " << sum_elements(fl1) << "\n";

    // ---- small addition ----
    if (!is_empty(fl1)) {
        std::cout << "First element: " << fl1.front() << "\n";
        std::cout << "Last element: " << last_element(fl1) << "\n";
        std::cout << "All even? "
                  << (all_even(fl1) ? "Yes\n" : "No\n");
    }
    print_divider();

    // Test case 2: all odd numbers
    std::forward_list<int> fl2 = {1, 3, 5, 7, 9};
    std::cout << "Original list 2: ";
    print_list(fl2);

    find_and_remove_odd(fl2);
    std::cout << "After removing odd numbers: ";
    print_list(fl2);

    std::cout << "Count: " << count_elements(fl2)
              << (is_empty(fl2) ? " (empty)\n" : "\n");
    print_divider();

    // Test case 3: all even numbers
    std::forward_list<int> fl3 = {2, 4, 6, 8, 10};
    std::cout << "Original list 3: ";
    print_list(fl3);

    find_and_remove_odd(fl3);
    std::cout << "After removing odd numbers: ";
    print_list(fl3);

    std::cout << "Count: " << count_elements(fl3) << "\n";
    std::cout << "Sum: " << sum_elements(fl3) << "\n";
    print_divider();

    // Test case 4: empty list
    std::forward_list<int> fl4;
    std::cout << "Original list 4 (empty): ";
    print_list(fl4);

    find_and_remove_odd(fl4);
    std::cout << "After removing odd numbers: ";
    print_list(fl4);

    std::cout << "Count: " << count_elements(fl4)
              << (is_empty(fl4) ? " (empty)\n" : "\n");

    // ---- very small addition ----
    fl3.clear();
    std::cout << "\nAfter clearing list 3, is empty? "
              << (is_empty(fl3) ? "Yes\n" : "No\n");
    // ----------------------------

    return 0;
}
