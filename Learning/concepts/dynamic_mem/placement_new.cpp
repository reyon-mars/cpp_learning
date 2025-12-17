// ======================================================
// EVEN MORE EXTRA CODE (still untouched above)
// ======================================================

#include <new>          // std::align_val_t, placement new
#include <type_traits>  // std::aligned_storage

// --------------------------------------
// 1. Alignment-safe placement new storage
// --------------------------------------

using BaseStorage = std::aligned_storage_t<
    sizeof(base),
    alignof(base)
>;

void test_aligned_storage() {
    std::cout << "\n[Extra] Aligned storage placement-new test\n";

    BaseStorage storage;
    base* b = new (&storage) base();

    b->~base(); // manual destruction
}

// --------------------------------------
// 2. Placement new with arrays (loop)
// --------------------------------------

void test_array_placement_new() {
    std::cout << "\n[Extra] Placement-new array construction\n";

    constexpr int N = 3;
    void* raw = operator new[](sizeof(base) * N);

    base* arr = static_cast<base*>(raw);

    for (int i = 0; i < N; ++i) {
        new (&arr[i]) base();
    }

    for (int i = N - 1; i >= 0; --i) {
        arr[i].~base();
    }

    operator delete[](raw);
}

// --------------------------------------
// 3. Virtual destructor demonstration
// --------------------------------------

class poly_base {
public:
    poly_base() {
        std::cout << "poly_base ctor\n";
    }
    virtual ~poly_base() {
        std::cout << "poly_base dtor\n";
    }
};

class poly_derived : public poly_base {
public:
    poly_derived() {
        std::cout << "poly_derived ctor\n";
    }
    ~poly_derived() {
        std::cout << "poly_derived dtor\n";
    }
};

void test_virtual_destructor() {
    std::cout << "\n[Extra] Virtual destructor test\n";

    poly_base* p = new poly_derived();
    delete p; // correct destruction chain
}

// --------------------------------------
// 4. RAII helper for placement-new objects
// --------------------------------------

template <typename T>
struct PlacementGuard {
    T* ptr;
    explicit PlacementGuard(T* p) : ptr(p) {}
    ~PlacementGuard() {
        if (ptr)
            ptr->~T();
    }
};

void test_raii_guard() {
    std::cout << "\n[Extra] Placement-new with RAII guard\n";

    alignas(base) char buffer[sizeof(base)];
    base* b = new (buffer) base();

    PlacementGuard<base> guard(b); // ensures destruction
}

// --------------------------------------
// Auto execution before main()
// --------------------------------------

struct MorePlacementTests {
    MorePlacementTests() {
        std::cout << "\n=== Running More Placement-New Tests ===\n";
        test_aligned_storage();
        test_array_placement_new();
        test_virtual_destructor();
        test_raii_guard();
    }
};

MorePlacementTests __more_placement_tests;
