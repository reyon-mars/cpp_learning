#include <forward_list>
#include <iostream>

// ======================================================
// FORWARD DECLARATION (minimal, required)
// ======================================================
int count_elements(const std::forward_list<int>& fl);

// ======================================================
// ORIGINAL CODE (UNCHANGED LOGIC)
// ======================================================

int main(void) {

    std::forward_list<int> f_lst = {0,1,2,3,4,5,6,7,8,9};
    auto prev = f_lst.before_begin();
    auto curr = f_lst.begin();

    while (curr != f_lst.end()) {
        if (*curr % 2) {
            curr = f_lst.erase_after(prev);
        } else {
            prev = curr;
            ++curr;
        }
    }

    for (auto elem : f_lst) {
        std::cout << elem << " ";
    }

    // ---- small added usage ----
    std::cout << "\nEven count: " << count_elements(f_lst) << std::endl;

    return 0;
}

// ======================================================
// MINIMAL EXTRA CODE (helper only)
// ======================================================

int count_elements(const std::forward_list<int>& fl) {
    int count = 0;
    for (int _ : fl) {
        (void)_;
        ++count;
    }
    return count;
}
