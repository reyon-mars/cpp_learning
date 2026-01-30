#include <forward_list>
#include <iostream>

// ======================================================
// FORWARD DECLARATIONS (minimal helpers)
// ======================================================
int count_elements(const std::forward_list<int>& fl);
bool is_empty(const std::forward_list<int>& fl);

// ---- very small extra helpers ----
int sum_elements(const std::forward_list<int>& fl);
int last_element(const std::forward_list<int>& fl);
// ---------------------------------

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
    for (auto elem : f_lst) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    // ---- small added usage ----
    std::cout << "Even count: " << count_elements(f_lst) << std::endl;
    std::cout << (is_empty(f_lst) ? "List is empty\n"
                                  : "List is not empty\n");

    if (!is_empty(f_lst)) {
        std::cout << "Last element: " << last_element(f_lst) << "\n";
        std::cout << "Sum of elements: " << sum_elements(f_lst) << "\n";
    }
    // --------------------------

    return 0;
}

// ======================================================
// MINIMAL EXTRA CODE (helpers only)
// ======================================================

int count_elements(const std::forward_list<int>& fl) {
    int count = 0;
    for (int _ : fl) {
        (void)_;
        ++count;
    }
    return count;
}

bool is_empty(const std::forward_list<int>& fl) {
    return fl.begin() == fl.end();
}

// ---- very small helpers ----
int sum_elements(const std::forward_list<int>& fl) {
    int sum = 0;
    for (int v : fl)
        sum += v;
    return sum;
}

int last_element(const std::forward_list<int>& fl) {
    int last = 0;
    for (int v : fl)
        last = v;
    return last;
}
