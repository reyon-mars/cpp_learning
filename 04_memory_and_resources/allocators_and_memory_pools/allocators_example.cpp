// Allocators and Memory Pools Exercise
// Custom allocation strategies

#include <iostream>
#include <memory>
#include <vector>
#include <new>
#include <cstddef>   // ✅ added

template<typename T>
class SimpleAllocator {
public:
    using value_type = T;

    SimpleAllocator() noexcept = default;

    template<typename U>
    SimpleAllocator(const SimpleAllocator<U>&) noexcept {}

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

    // -------- NEW ADDITIONS --------

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

    bool operator==(const SimpleAllocator&) const { return true; }
    bool operator!=(const SimpleAllocator&) const { return false; }

    // --------------------------------
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

    // ----------- NEW ADDITIONS -----------

    std::size_t capacity() const {
        return pool.size();
    }

    std::size_t used() const {
        return index;
    }

    bool isFull() const {
        return index >= pool.size();
    }

    // ------------------------------------
};
// -----------------------------------


// ----------- NEW ADDITION -----------
// Custom object to test allocator
struct Test {
    int x;
    Test(int v) : x(v) {
        std::cout << "Test constructed: " << x << "\n";
    }
    ~Test() {
        std::cout << "Test destroyed: " << x << "\n";
    }
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
    alloc.construct(t, 42);

    std::cout << "Value inside Test: " << t->x << "\n";

    alloc.destroy(t);
    alloc.deallocate(t, 1);

    // ----------------------------------

    std::cout << "\n--- End of main ---\n";

    return 0;
}
