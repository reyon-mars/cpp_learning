#include <list>
#include <iostream>
#include <algorithm>
#include <numeric>   // tiny addition for accumulate

// ---------------- ORIGINAL LOGIC ----------------

void process_original_list() {
    std::list<int> lst = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    auto iter = lst.begin();

    while (iter != lst.end()) {
        if (*iter % 2) {                    // odd
            iter = lst.insert(iter, *iter); // duplicate odd
            std::advance(iter, 2);
        } else {                            // even
            iter = lst.erase(iter);         // remove even
        }
    }

    std::cout << "Original processed list: ";
    for (auto item : lst) {
        std::cout << item << " ";
    }
    std::cout << "\n";

    // ---- small additions ----
    std::cout << "Final size: " << lst.size() << "\n";
    std::cout << "First element: " << lst.front() << "\n";
    std::cout << "Last element: " << lst.back() << "\n";
}

// ---------------- SMALL EXTRA HELPERS ----------------

// Print list
void print_list(const std::list<int>& L) {
    for (int v : L)
        std::cout << v << " ";
    std::cout << "\n";
}

// Count odd numbers
std::size_t count_odds(const std::list<int>& L) {
    std::size_t count = 0;
    for (int v : L)
        if (v % 2) ++count;
    return count;
}

// Count even numbers
std::size_t count_evens(const std::list<int>& L) {
    return L.size() - count_odds(L);
}

// ---- VERY SMALL EXTRA HELPERS ----

// Sum of elements
int sum_list(const std::list<int>& L) {
    return std::accumulate(L.begin(), L.end(), 0);
}

// Max element (safe)
int max_list(const std::list<int>& L) {
    return L.empty() ? 0 : *std::max_element(L.begin(), L.end());
}

// Divider
void print_divider() {
    std::cout << "----------------------\n";
}

// ---------------- MAIN ----------------

int main(void) {

    // ---------- ORIGINAL BEHAVIOR ----------
    process_original_list();

    print_divider();

    // ---------- SMALL EXTRA DEMO ----------
    std::list<int> demo = { 2, 4, 6, 8 };

    std::cout << "Demo list: ";
    print_list(demo);

    std::cout << "Demo list size: " << demo.size() << "\n";
    std::cout << (demo.empty() ? "Demo list is empty\n"
                               : "Demo list is not empty\n");

    std::cout << "Odd count: " << count_odds(demo) << "\n";
    std::cout << "Even count: " << count_evens(demo) << "\n";

    // ---- tiny extra usage ----
    std::cout << "Sum of demo list: " << sum_list(demo) << "\n";
    std::cout << "Max value in demo list: " << max_list(demo) << "\n";

    return 0;
}
