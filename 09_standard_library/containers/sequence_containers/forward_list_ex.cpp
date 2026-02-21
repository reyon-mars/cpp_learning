#include <forward_list>
#include <iostream>

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

    // ==================================================

    return 0;
}
