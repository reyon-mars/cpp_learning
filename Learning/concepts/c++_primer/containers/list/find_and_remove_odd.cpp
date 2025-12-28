#include <forward_list>
#include <iostream>
#include <cstddef>

// ------------------------------------------------
// ORIGINAL CODE (UNCHANGED)
// ------------------------------------------------

void find_and_remove_odd(std::forward_list<int>& lst) {
    auto prev = lst.before_begin();
    auto curr = lst.begin();

    while (curr != lst.end()) {
        if (*curr % 2) {
            curr = lst.erase_after(prev);
        } else {
            prev = curr;
            ++curr;
        }
    }
}

void print_list(std::forward_list<int>& lst) {
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

    return 0;
}

// ------------------------------------------------
// EXTRA CODE (append only)
// ------------------------------------------------

// Generate a pseudo-random forward_list
std::forward_list<int> generate_random_list(int n) {
    std::forward_list<int> lst;
    for (int i = n; i >= 1; --i)
        lst.push_front((i * 7) % 13);
    return lst;
}

// Count elements in forward_list
std::size_t count_elements(const std::forward_list<int>& lst) {
    std::size_t count = 0;
    for (int v : lst) {
        (void)v;
        ++count;
    }
    return count;
}

// Verify list contains only even numbers
bool verify_only_even(const std::forward_list<int>& lst) {
    for (int v : lst)
        if (v % 2 != 0)
            return false;
    return true;
}

// Alternative odd removal using remove_if
void remove_odd_using_remove_if(std::forward_list<int>& lst) {
    lst.remove_if([](int x) {
        return x % 2 != 0;
    });
}

// Duplicate a forward_list
std::forward_list<int> duplicate_list(const std::forward_list<int>& src) {
    std::forward_list<int> dst;
    auto it = dst.before_begin();
    for (int v : src) {
        it = dst.insert_after(it, v);
    }
    return dst;
}

// Extra test runner
void test_random_list() {
    auto lst = generate_random_list(10);

    std::cout << "\n[Extra] Random list: ";
    print_list(lst);

    find_and_remove_odd(lst);

    std::cout << "[Extra] After removing odd numbers: ";
    print_list(lst);

    std::cout << "[Extra] Count = " << count_elements(lst) << "\n";
    std::cout << "[Extra] Only even? "
              << (verify_only_even(lst) ? "YES" : "NO") << "\n";
}

// Automatic validation before main()
int ___ = []() {
    std::cout << "\n[Extra] --- Running validation tests ---\n";

    std::forward_list<int> base = {1,2,3,4,5,6,7,8,9};

    auto manual  = duplicate_list(base);
    auto builtin = duplicate_list(base);

    find_and_remove_odd(manual);
    remove_odd_using_remove_if(builtin);

    std::cout << "[Extra] Manual removal: ";
    print_list(manual);

    std::cout << "[Extra] remove_if removal: ";
    print_list(builtin);

    std::cout << "[Extra] Only even (manual)? "
              << (verify_only_even(manual) ? "YES" : "NO") << "\n";

    test_random_list();
    return 0;
}();

