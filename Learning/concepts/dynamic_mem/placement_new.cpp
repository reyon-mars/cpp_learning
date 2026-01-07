#include <iostream>
#include <new>
#include <type_traits>

// ---------------- ORIGINAL CODE (UNCHANGED) ----------------

class base {
private:
    int a;

public:
    base() { std::cout << "Constructor\n"; }
    ~base() { std::cout << "Destructor\n"; }
};

int main(void) {
    std::cout << "Normal case\n";
    base* obj = new base();
    delete obj;

    std::cout << "\nPlacement New Case\n";

    alignas(base) char memory[sizeof(base) * 2];

    base* obj1 = new (&memory[0]) base();
    base* obj2 = new (&memory[sizeof(base)]) base();

    obj1->~base();
    obj2->~base();

    return 0;
}

// ----------------------------------------------------------
// EXTRA CODE ADDED BELOW (original code untouched)
// ----------------------------------------------------------

// Derived class demo
class derived : public base {
public:
    derived() {
        std::cout << "Derived Constructor\n";
    }
    ~derived() {
        std::cout << "Derived Destructor\n";
    }
};

// Virtual destructor demonstration
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
    ~poly_derived() override {
        std::cout << "poly_derived dtor\n";
    }
};

// Normal heap allocation test
void test_normal_allocation() {
    std::cout << "\n[Extra] Normal Allocation (derived)\n";
    derived* d = new derived();
    delete d;
}

// Placement-new allocation test
void test_placement_new() {
    std::cout << "\n[Extra] Placement New (derived)\n";

    alignas(derived) char mem[sizeof(derived) * 2];

    derived* d1 = new (&mem[0]) derived();
    derived* d2 = new (&mem[sizeof(derived)]) derived();

    d1->~derived();
    d2->~derived();
}

// Alignment-safe placement new
void test_aligned_storage() {
    std::cout << "\n[Extra] Aligned Storage Placement-New\n";

    using Storage = std::aligned_storage_t<sizeof(base), alignof(base)>;
    Storage storage;

    base* b = new (&storage) base();
    b->~base();
}

// Placement-new array demo
void test_array_placement_new() {
    std::cout << "\n[Extra] Placement-New Array\n";

    constexpr int N = 3;
    alignas(base) char buffer[sizeof(base) * N];

    base* arr = reinterpret_cast<base*>(buffer);

    for (int i = 0; i < N; ++i)
        new (&arr[i]) base();

    for (int i = N - 1; i >= 0; --i)
        arr[i].~base();
}

// Virtual destructor test
void test_virtual_destructor() {
    std::cout << "\n[Extra] Virtual Destructor Test\n";
    poly_base* p = new poly_derived();
    delete p;
}

// RAII guard for placement new
template <typename T>
struct PlacementGuard {
    T* ptr;
    explicit PlacementGuard(T* p) : ptr(p) {}
    ~PlacementGuard() {
        if (ptr) ptr->~T();
    }
};

void test_raii_guard() {
    std::cout << "\n[Extra] Placement-New with RAII Guard\n";

    alignas(base) char buffer[sizeof(base)];
    base* b = new (buffer) base();

    PlacementGuard<base> guard(b);
}

// ----------------------------------------------------------
// AUTO-EXECUTED EXTRA TESTS (RUN BEFORE main())
// ----------------------------------------------------------

struct AutoRunPlacementTests {
    AutoRunPlacementTests() {
        std::cout << "\n=== Running Extra Placement-New Tests ===\n";
        test_normal_allocation();
        test_placement_new();
        test_aligned_storage();
        test_array_placement_new();
        test_virtual_destructor();
        test_raii_guard();
    }
};

AutoRunPlacementTests __auto_run_tests;
