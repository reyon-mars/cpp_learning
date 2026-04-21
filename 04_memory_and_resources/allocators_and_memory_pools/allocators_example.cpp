#include <iostream>
#include <memory>
#include <vector>
#include <new>
#include <cstddef>

// ----------- UPDATED ALLOCATOR -----------

template<typename T>
class SimpleAllocator {
public:
    using value_type = T;

    SimpleAllocator() noexcept = default;

    template<typename U>
    SimpleAllocator(const SimpleAllocator<U>&) noexcept {}

    // ✅ ADDED: rebind
    template<typename U>
    struct rebind {
        using other = SimpleAllocator<U>;
    };

    T* allocate(std::size_t n) {
        std::cout << "[Allocator] Allocating "
                  << n << " object(s) ("
                  << n * sizeof(T) << " bytes)\n";

        return static_cast<T*>(::operator new(n * sizeof(T)));
    }

    void deallocate(T* p, std::size_t n) noexcept {
        std::cout << "[Allocator] Deallocating "
                  << n << " object(s)\n";

        ::operator delete(p);
    }

    // -------- EXISTING ADDITIONS --------

    template<typename U, typename... Args>
    void construct(U* p, Args&&... args) {
        std::cout << "[Allocator] Constructing object\n";
        ::new ((void*)p) U(std::forward<Args>(args)...);
    }

    template<typename U>
    void destroy(U* p) {
        std::cout << "[Allocator] Destroying object\n";
        p->~U();
    }

    // ✅ ADDED: max_size
    std::size_t max_size() const noexcept {
        return std::size_t(-1) / sizeof(T);
    }

    bool operator==(const SimpleAllocator&) const { return true; }
    bool operator!=(const SimpleAllocator&) const { return false; }
};

// -------- SIMPLE MEMORY POOL --------
class IntPool {
private:
    std::vector<int> pool;
    std::size_t index = 0;

public:
    IntPool(std::size_t size) : pool(size) {}

    int* allocate() {
        if (index >= pool.size()) {
            throw std::bad_alloc();
        }
        std::cout << "[Pool] Allocating index " << index << "\n";
        return &pool[index++];
    }

    void reset() {
        std::cout << "[Pool] Reset\n";
        index = 0;
    }

    std::size_t capacity() const { return pool.size(); }
    std::size_t used() const { return index; }
    bool isFull() const { return index >= pool.size(); }
};

// ----------- NEW ADDITION -----------
struct Test {
    int x;
    Test(int v) : x(v) {
        std::cout << "Test constructed: " << x << "\n";
    }
    ~Test() {
        std::cout << "Test destroyed: " << x << "\n";
    }
};

// ✅ ADDED: Generic Object Pool
template<typename T>
class ObjectPool {
private:
    std::vector<std::aligned_storage_t<sizeof(T), alignof(T)>> buffer;
    std::size_t index = 0;

public:
    ObjectPool(std::size_t size) : buffer(size) {}

    template<typename... Args>
    T* create(Args&&... args) {
        if (index >= buffer.size()) {
            throw std::bad_alloc();
        }

        void* place = &buffer[index++];
        std::cout << "[ObjectPool] Creating object at slot "
                  << index - 1 << "\n";

        return new (place) T(std::forward<Args>(args)...);
    }

    void reset() {
        std::cout << "[ObjectPool] Reset (no destructors called!)\n";
        index = 0;
    }
};

// ✅ ADDED: RAII guard
class ScopeGuard {
public:
    ScopeGuard() { std::cout << "[ScopeGuard] Enter\n"; }
    ~ScopeGuard() { std::cout << "[ScopeGuard] Exit\n"; }
};

// ------------------------------------

int main() {

    std::vector<int, SimpleAllocator<int>> vec;

    std::cout << "\n--- Pushing Elements ---\n";

    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);

    std::cout << "\n--- Reserving More Capacity ---\n";
    vec.reserve(10);

    // -------- NEW FEATURE USAGE --------

    std::cout << "\n--- Memory Pool Demo ---\n";

    IntPool pool(3);

    int* a = pool.allocate();
    int* b = pool.allocate();
    int* c = pool.allocate();

    *a = 10;
    *b = 20;
    *c = 30;

    std::cout << "Pool values: "
              << *a << " " << *b << " " << *c << "\n";

    std::cout << "Pool used: " << pool.used()
              << "/" << pool.capacity() << "\n";

    std::cout << "Pool full? "
              << (pool.isFull() ? "Yes\n" : "No\n");

    pool.reset();

    // -------- Allocator with custom type --------

    std::cout << "\n--- Custom Allocator with Objects ---\n";

    SimpleAllocator<Test> alloc;

    Test* t = alloc.allocate(1);

    // ✅ modern usage
    std::allocator_traits<SimpleAllocator<Test>>::construct(alloc, t, 42);

    std::cout << "Value inside Test: " << t->x << "\n";

    std::allocator_traits<SimpleAllocator<Test>>::destroy(alloc, t);
    alloc.deallocate(t, 1);

    // -------- NEW: Object Pool --------

    std::cout << "\n--- Object Pool Demo ---\n";

    ScopeGuard guard;

    ObjectPool<Test> objPool(2);

    Test* p1 = objPool.create(100);
    Test* p2 = objPool.create(200);

    std::cout << "Pool objects: "
              << p1->x << ", " << p2->x << "\n";

    objPool.reset();

    // ----------------------------------

    std::cout << "\n--- End of main ---\n";

    return 0;
}
