#include <forward_list>
#include <iostream>

// ======================================================
// FORWARD DECLARATIONS (minimal, required)
// ======================================================
int count_elements(const std::forward_list<int>& fl);
bool is_empty(const std::forward_list<int>& fl);

// ======================================================
// ORIGINAL CODE (UNCHANGED LOGIC)
// ======================================================

int main(void) #include <forward_list>
#include <iostream>

// ======================================================
// FORWARD DECLARATIONS (minimal, required)
// ======================================================
int count_elements(const std::forward_list<int>& fl);
bool is_empty(const std::forward_list<int>& fl);

// ======================================================
// ORIGINAL CODE (UNCHANGED LOGIC)
// ======================================================

int main(void) {

    std::forward_list<int> f_lst = {0,1,2,3,4,5,6,7,8,9};
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

    for (auto elem : f_lst) {
        std::cout << elem << " ";
    }

    // ---- small added usage ----
    std::cout << "\nEven count: " << count_elements(f_lst) << std::endl;
    std::cout << (is_empty(f_lst) ? "List is empty\n"
                                  : "List is not empty\n");
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
{

    std::forward_list<int> f_lst = {0,1,2,3,4,5,6,7,8,9};
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

    for (auto elem : f_lst) {
        std::cout << elem << " ";
    }

    // ---- small added usage ----
    std::cout << "\nEven count: " << count_elements(f_lst) << std::endl;
    std::cout << (is_empty(f_lst) ? "List is empty\n"
                                  : "List is not empty\n");
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
