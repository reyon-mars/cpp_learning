// ======================================================
// EVEN MORE EXTRA CODE (original code still untouched)
// ======================================================

// Count elements in a forward_list
std::size_t count_elements(const std::forward_list<int>& lst) {
    std::size_t count = 0;
    for (int v : lst) (void)v, ++count;
    return count;
}

// Verify that list contains ONLY even numbers
bool verify_only_even(const std::forward_list<int>& lst) {
    for (int v : lst)
        if (v % 2 != 0)
            return false;
    return true;
}

// Alternative odd-removal using remove_if (for comparison)
void remove_odd_using_remove_if(std::forward_list<int>& lst) {
    lst.remove_if([](int x) {
        return x % 2 != 0;
    });
}

// Duplicate a list (explicit traversal)
std::forward_list<int> duplicate_list(const std::forward_list<int>& src) {
    std::forward_list<int> dst;
    auto it = dst.before_begin();
    for (int v : src) {
        it = dst.insert_after(it, v);
    }
    return dst;
}

// ------------------------------------------------------
// Automatic validation tests (run before main)
// ------------------------------------------------------

int ____extra_validation = [](){
    std::cout << "\n[Extra] === Validation Tests ===\n";

    std::forward_list<int> base = {1,2,3,4,5,6,7,8,9};

    std::cout << "[Extra] Base list: ";
    print_list(base);

    auto manual = duplicate_list(base);
    auto builtin = duplicate_list(base);

    find_and_remove_odd(manual);
    remove_odd_using_remove_if(builtin);

    std::cout << "[Extra] Manual removal result: ";
    print_list(manual);

    std::cout << "[Extra] remove_if result: ";
    print_list(builtin);

    std::cout << "[Extra] Manual count = " 
              << count_elements(manual) << "\n";

    std::cout << "[Extra] Only even? "
              << (verify_only_even(manual) ? "YES" : "NO") << "\n";

    return 0;
}();
