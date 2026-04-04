// STL Internals Exploration
// Understanding how STL containers are implemented

#include <iostream>
#include <vector>

// ✅ ADDED
#include <deque>

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
    std::cout << "Usage efficiency: " << efficiency << "%\n";

    return 0;
}
