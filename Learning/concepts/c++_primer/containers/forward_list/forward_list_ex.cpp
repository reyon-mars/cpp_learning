// ======================================================
// EVEN MORE EXTRA CODE (original code still untouched)
// ======================================================

// Remove elements greater than a threshold
void remove_greater_than(std::forward_list<int>& fl, int limit) {
    auto prev = fl.before_begin();
    auto curr = fl.begin();

    while (curr != fl.end()) {
        if (*curr > limit) {
            curr = fl.erase_after(prev);
        } else {
            prev = curr;
            ++curr;
        }
    }
}

// Square all elements in-place
void square_elements(std::forward_list<int>& fl) {
    for (int& v : fl) {
        v *= v;
    }
}

// Count elements matching a predicate
template<typename Pred>
int count_if_fl(const std::forward_list<int>& fl, Pred p) {
    int count = 0;
    for (int v : fl)
        if (p(v)) ++count;
    return count;
}

// Merge two sorted forward_lists
std::forward_list<int> merge_sorted(
    std::forward_list<int> a,
    std::forward_list<int> b
) {
    a.merge(b);
    return a;
}

// --------------------------------------
// Auto-executed extra tests (before main)
// --------------------------------------

int ___extra_tests = [](){
    std::cout << "\n=== More forward_list Tests ===\n";

    std::forward_list<int> fl = {1, 3, 5, 7, 9, 2, 4, 6};

    std::cout << "[Extra] Original: ";
    for (int v : fl) std::cout << v << " ";
    std::cout << "\n";

    remove_greater_than(fl, 5);
    std::cout << "[Extra] After removing > 5: ";
    for (int v : fl) std::cout << v << " ";
    std::cout << "\n";

    square_elements(fl);
    std::cout << "[Extra] After squaring: ";
    for (int v : fl) std::cout << v << " ";
    std::cout << "\n";

    int even_count = count_if_fl(fl, [](int x){ return x % 2 == 0; });
    std::cout << "[Extra] Even count: " << even_count << "\n";

    std::forward_list<int> a = {1, 3, 5};
    std::forward_list<int> b = {2, 4, 6};

    auto merged = merge_sorted(a, b);
    std::cout << "[Extra] Merged sorted list: ";
    for (int v : merged) std::cout << v << " ";
    std::cout << "\n";

    return 0;
}();
