#include <iostream>
#include <vector>
#include <deque>
#include <list>
#include <array>
#include <numeric>
#include <algorithm>
#include <iomanip>
#include <span>
#include <string_view>

void print_growth(std::size_t old_cap, std::size_t new_cap) {
    if (old_cap > 0)
        std::cout << " [growth x" << static_cast<double>(new_cap) / static_cast<double>(old_cap) << "]";
}

[[nodiscard]] int count_reallocations(int n = 50) {
    std::vector<int> v;
    const int* prev = nullptr;
    int count = 0;
    for (int i = 0; i < n; ++i) {
        v.push_back(i);
        if (prev && prev != v.data()) ++count;
        prev = v.data();
    }
    return count;
}

void print_stats(std::span<const int> v) {
    if (v.empty()) { std::cout << "(empty)\n"; return; }
    const int sum = std::accumulate(v.begin(), v.end(), 0);
    std::cout << "sum=" << sum
              << " avg=" << static_cast<double>(sum) / static_cast<double>(v.size()) << "\n";
}

void compare_reserve_behavior() {
    std::cout << "\n=== reserve vs no reserve ===\n";
    std::vector<int> a, b;
    b.reserve(100);
    for (int i = 0; i < 100; ++i) { a.push_back(i); b.push_back(i); }
    std::cout << "without reserve: capacity=" << a.capacity() << "\n"
              << "with reserve:    capacity=" << b.capacity() << "\n";
}

void array_demo() {
    std::cout << "\n=== std::array ===\n";
    constexpr std::array<int, 5> arr{1, 2, 3, 4, 5};
    for (int v : arr) std::cout << v << " ";
    std::cout << "\n";
}

int main() {
    std::cout << "=== Vector growth tracking ===\n";
    std::vector<int> vec;
    std::cout << "initial: capacity=" << vec.capacity() << " size=" << vec.size() << "\n";

    const int* prev_ptr = nullptr;
    std::size_t prev_cap = vec.capacity();

    for (int i = 0; i < 10; ++i) {
        vec.push_back(i);
        const int* cur = vec.data();
        std::cout << "push_back(" << i << "): capacity=" << vec.capacity()
                  << " size=" << vec.size();
        if (prev_ptr && prev_ptr != cur)
            std::cout << " <-- reallocation";
        if (vec.capacity() != prev_cap) {
            std::cout << " [" << prev_cap << " → " << vec.capacity() << "]";
            print_growth(prev_cap, vec.capacity());
            prev_cap = vec.capacity();
        }
        std::cout << "\n";
        prev_ptr = cur;
    }

    vec.reserve(100);
    std::cout << "After reserve(100): capacity=" << vec.capacity() << "\n";
    vec.shrink_to_fit();
    std::cout << "After shrink_to_fit: capacity=" << vec.capacity() << "\n";

    std::cout << "\n=== Iterator invalidation ===\n";
    std::vector<int> v2{1, 2, 3};
    auto it = v2.begin();
    std::cout << "Before push_back: *it=" << *it << "\n";
    v2.push_back(4);
    std::cout << "After push_back (it may be invalidated — do not dereference it)\n";

    std::cout << "\n=== deque (no single contiguous block) ===\n";
    std::deque<int> dq;
    for (int i = 0; i < 5; ++i) { dq.push_back(i); std::cout << "size=" << dq.size() << "\n"; }

    std::cout << "\n=== Memory efficiency ===\n";
    const double eff = static_cast<double>(vec.size()) / static_cast<double>(vec.capacity()) * 100.0;
    std::cout << "size=" << vec.size() << " capacity=" << vec.capacity()
              << " efficiency=" << std::fixed << std::setprecision(2) << eff << "%\n";

    std::cout << "\n=== Contiguous memory ===\n";
    for (std::size_t i = 0; i + 1 < vec.size(); ++i)
        std::cout << static_cast<void*>(&vec[i]) << " → " << static_cast<void*>(&vec[i+1]) << "\n";

    std::cout << "\n=== push_back vs emplace_back ===\n";
    std::vector<std::pair<int,int>> pairs;
    pairs.push_back({1, 2});
    pairs.emplace_back(3, 4);
    std::cout << "pairs size=" << pairs.size() << "\n";

    compare_reserve_behavior();

    std::cout << "\n=== Reallocation count (50 inserts) ===\n";
    std::cout << "reallocations=" << count_reallocations() << "\n";

    std::cout << "\n=== Vector stats ===\n";
    print_stats(vec);
    if (!vec.empty())
        std::cout << "front=" << vec.front() << " back=" << vec.back() << "\n";

    std::cout << "\n=== Container size comparison ===\n";
    std::cout << "vector(10).size="  << std::vector<int>(10).size() << "\n"
              << "list(10).size="    << std::list<int>(10).size()   << "\n";

    array_demo();

    std::cout << "\n=== max_element ===\n";
    if (auto m = std::ranges::max_element(vec); m != vec.end())
        std::cout << "max=" << *m << "\n";

    std::cout << "\n=== clear ===\n";
    std::vector<int> temp = vec;
    temp.clear();
    std::cout << "after clear: size=" << temp.size() << " capacity=" << temp.capacity() << "\n";

    return 0;
}
