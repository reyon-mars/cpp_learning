// STL Internals Exploration
// Understanding how STL containers are implemented

#include <iostream>
#include <vector>

// ✅ ADDED
#include <deque>
#include <numeric>   // ✅ ADDED
#include <iomanip>   // ✅ ADDED

// -------- NEW ADDITIONS --------

// Show growth factor
void print_growth(size_t old_cap, size_t new_cap) {
    if (old_cap > 0) {
        double factor = (double)new_cap / old_cap;
        std::cout << "  [growth factor: " << factor << "]";
    }
}

// Compare reserve vs no reserve
void compare_reserve_behavior() {
    std::vector<int> a, b;

    for (int i = 0; i < 100; ++i)
        a.push_back(i);

    b.reserve(100);
    for (int i = 0; i < 100; ++i)
        b.push_back(i);

    std::cout << "\nReserve vs No Reserve:\n";
    std::cout << "Without reserve capacity: " << a.capacity() << "\n";
    std::cout << "With reserve capacity: " << b.capacity() << "\n";
}

// ✅ ADDED: simple reallocation counter
int count_reallocations() {
    std::vector<int> v;
    const int* prev = nullptr;
    int reallocs = 0;

    for (int i = 0; i < 50; ++i) {
        v.push_back(i);
        if (prev && prev != v.data())
            ++reallocs;
        prev = v.data();
    }
    return reallocs;
}

// ✅ ADDED: print small stats
void print_stats(const std::vector<int>& v) {
    int sum = std::accumulate(v.begin(), v.end(), 0);
    double avg = v.empty() ? 0.0 : (double)sum / v.size();

    std::cout << "Sum: " << sum << "\n";
    std::cout << "Average: " << avg << "\n";
}

// --------------------------------

int main() {
    std::vector<int> vec;

    std::cout << "Vector capacity: " << vec.capacity() << "\n";
    std::cout << "Vector size: " << vec.size() << "\n";

    const int* previous_address = nullptr;

    // ✅ ADDED: Track previous capacity
    size_t prev_capacity = vec.capacity();

    for (int i = 0; i < 10; ++i) {

        vec.push_back(i);

        const int* current_address = vec.data();

        std::cout << "After push_back(" << i << "): "
                  << "capacity=" << vec.capacity()
                  << ", size=" << vec.size()
                  << ", data=" << current_address;

        // Detect reallocation
        if (previous_address && previous_address != current_address) {
            std::cout << "  <-- reallocation happened";
        }

        // ✅ ADDED: Detect growth pattern
        if (vec.capacity() != prev_capacity) {
            std::cout << "  [capacity grew from " 
                      << prev_capacity << " to " 
                      << vec.capacity() << "]";
            
            // ✅ NEW: growth factor
            print_growth(prev_capacity, vec.capacity());

            prev_capacity = vec.capacity();
        }

        std::cout << "\n";

        previous_address = current_address;
    }

    // Reserve memory
    vec.reserve(100);
    std::cout << "After reserve(100): capacity=" << vec.capacity() << "\n";

    // Shrink to fit
    vec.shrink_to_fit();
    std::cout << "After shrink_to_fit: capacity=" << vec.capacity() << "\n";

    // ----------------------------------------------------
    // ✅ ADDED: Iterator invalidation demo
    std::cout << "\nIterator Invalidation Demo:\n";
    std::vector<int> v2 = {1, 2, 3};

    auto it = v2.begin();
    std::cout << "Before push_back, first element: " << *it << "\n";

    v2.push_back(4); // may invalidate iterator

    std::cout << "After push_back, iterator may be invalidated!\n";
    std::cout << "(Do NOT dereference 'it' after reallocation)\n";

    // ----------------------------------------------------
    // ✅ ADDED: Compare with deque
    std::cout << "\nDeque Comparison:\n";
    std::deque<int> dq;

    for (int i = 0; i < 10; ++i) {
        dq.push_back(i);
        std::cout << "Deque size: " << dq.size() << "\n";
    }

    std::cout << "Deque does not reallocate like vector (no single contiguous block)\n";

    // ----------------------------------------------------
    // ✅ ADDED: Memory efficiency
    std::cout << "\nMemory Efficiency:\n";
    std::cout << "Vector size: " << vec.size() << "\n";
    std::cout << "Vector capacity: " << vec.capacity() << "\n";

    double efficiency = (double)vec.size() / vec.capacity() * 100.0;
    std::cout << "Usage efficiency: " << std::fixed << std::setprecision(2)
              << efficiency << "%\n";

    // ----------------------------------------------------
    // ✅ NEW: Contiguous memory proof
    std::cout << "\nContiguous Memory Check:\n";
    for (size_t i = 0; i < vec.size() - 1; ++i) {
        std::cout << &vec[i] << " -> " << &vec[i + 1] << "\n";
    }

    std::cout << "Addresses are sequential → vector is contiguous\n";

    // ----------------------------------------------------
    // ✅ NEW: push_back vs emplace_back
    std::cout << "\nPush vs Emplace Demo:\n";
    std::vector<std::pair<int,int>> pairs;

    pairs.push_back({1,2});      // temporary object
    pairs.emplace_back(3,4);     // constructed in-place

    std::cout << "pairs size: " << pairs.size() << "\n";

    // ----------------------------------------------------
    // ✅ NEW: Reserve comparison
    compare_reserve_behavior();

    // ----------------------------------------------------
    // ✅ VERY SMALL EXTRA ADDITIONS

    std::cout << "\nReallocation count (first 50 inserts): "
              << count_reallocations() << "\n";

    std::cout << "\nVector stats:\n";
    print_stats(vec);

    // ----------------------------------------------------

    return 0;
}
