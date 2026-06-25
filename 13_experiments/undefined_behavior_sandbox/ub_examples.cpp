#include <iostream>
#include <climits>
#include <limits>
#include <vector>
#include <array>
#include <optional>
#include <memory>
#include <cstddef>

void safe_division(int a, int b) {
    if (b == 0) { std::cout << "Division by zero — would be UB\n"; return; }
    std::cout << "Division " << a << "/" << b << "=" << a / b << "\n";
}

void safe_narrowing(long long value) {
    if (value > static_cast<long long>(std::numeric_limits<int>::max()) ||
        value < static_cast<long long>(std::numeric_limits<int>::min())) {
        std::cout << value << " overflows int\n";
    } else {
        std::cout << "Narrowed: " << static_cast<int>(value) << "\n";
    }
}

void smart_pointer_demo() {
    auto ptr = std::make_unique<int>(99);
    std::cout << "unique_ptr value=" << *ptr << "\n";
    ptr.reset();
    std::cout << "After reset ptr=" << std::boolalpha << (ptr == nullptr) << "\n";
}

int main() {
    std::cout << "=== Integer limits ===\n";
    std::cout << "INT_MAX=" << INT_MAX << " INT_MIN=" << INT_MIN << "\n"
              << "numeric_limits<int>::max()=" << std::numeric_limits<int>::max() << "\n";

    int a = INT_MAX;
    if (a > INT_MAX - 1)
        std::cout << "Adding 1 to INT_MAX would be signed overflow (UB)\n";

    std::cout << "\n=== Dangling pointer (no dereference) ===\n";
    {
        int* raw = new int{42};
        delete raw;
        raw = nullptr;
        std::cout << "After delete + null: raw=" << std::boolalpha << (raw == nullptr) << "\n";
    }

    std::cout << "\n=== Uninitialized variable (declared, not used) ===\n";
    [[maybe_unused]] int x;
    std::cout << "Declared but not read — avoids UB\n";

    std::cout << "\n=== Safe bounds check (vector::at) ===\n";
    std::vector<int> v{1, 2, 3};
    try {
        std::cout << "v.at(5)=";
        std::cout << v.at(5) << "\n";
    } catch (const std::out_of_range&) {
        std::cout << "out_of_range caught\n";
    }

    std::cout << "\n=== std::optional ===\n";
    std::optional<int> opt;
    if (!opt.has_value()) std::cout << "opt is empty — avoids UB\n";
    opt = 10;
    std::cout << "opt=" << opt.value() << "\n";

    std::cout << "\n=== std::array bounds check ===\n";
    std::array<int, 3> arr{1, 2, 3};
    try {
        std::cout << "arr.at(5)=";
        std::cout << arr.at(5) << "\n";
    } catch (const std::out_of_range&) {
        std::cout << "out_of_range caught\n";
    }

    std::cout << "\n=== Division safety ===\n";
    safe_division(10, 2);
    safe_division(10, 0);

    std::cout << "\n=== Iterator invalidation ===\n";
    std::vector<int> v2{1, 2, 3};
    [[maybe_unused]] auto it = v2.begin();
    v2.push_back(4);
    std::cout << "push_back may invalidate iterators — do not dereference it\n";

    std::cout << "\n=== Signed/unsigned mismatch ===\n";
    const int neg = -1;
    const unsigned int u = 1;
    if (static_cast<unsigned int>(neg) < u)
        std::cout << "With implicit conversion -1 becomes a large unsigned value\n";
    if (neg < static_cast<int>(u))
        std::cout << "Explicit cast to signed: -1 < 1 as expected\n";

    std::cout << "\n=== Alignment ===\n";
    std::cout << "alignof(int)=" << alignof(int)
              << " alignof(double)=" << alignof(double) << "\n";

    std::cout << "\n=== Narrowing conversion ===\n";
    safe_narrowing(100);
    safe_narrowing(999'999'999'999LL);

    std::cout << "\n=== Smart pointer ===\n";
    smart_pointer_demo();

    std::cout << "\n=== Null pointer check ===\n";
    const int* safe_ptr = nullptr;
    if (safe_ptr == nullptr)
        std::cout << "Null pointer detected safely\n";

    std::cout << "\n=== Safe iteration ===\n";
    for (std::size_t i = 0; i < v.size(); ++i) std::cout << v[i] << " ";
    std::cout << "\n";

    return 0;
}
