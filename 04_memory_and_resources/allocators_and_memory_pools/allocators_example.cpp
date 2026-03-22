// Allocators and Memory Pools Exercise
// Custom allocation strategies

#include <iostream>
#include <memory>
#include <vector>
#include <new>

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

    // Construct object in allocated memory
    template<typename U, typename... Args>
    void construct(U* p, Args&&... args) {
        std::cout << "[Allocator] Constructing object\n";
        ::new ((void*)p) U(std::forward<Args>(args)...);
    }

    // Destroy object
    template<typename U>
    void destroy(U* p) {
        std::cout << "[Allocator] Destroying object\n";
        p->~U();
    }

    // Allocator equality (required by STL)
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
};
// -----------------------------------

int main() {

    std::vector<int, SimpleAllocator<int>> vec;

    std::cout << "\n--- Pushing Elements ---\n";

    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);

    std::cout << "\n--- Reserving More Capacity ---\n";
    vec.reserve(10);   // forces reallocation

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

    pool.reset();

    // ----------------------------------

    std::cout << "\n--- End of main ---\n";

    return 0;
}
