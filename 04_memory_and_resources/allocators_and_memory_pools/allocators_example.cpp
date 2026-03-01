// Allocators and Memory Pools Exercise
// Custom allocation strategies

#include <iostream>
#include <memory>
#include <vector>

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
};

int main() {

    std::vector<int, SimpleAllocator<int>> vec;

    std::cout << "\n--- Pushing Elements ---\n";

    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);

    std::cout << "\n--- Reserving More Capacity ---\n";
    vec.reserve(10);   // forces reallocation

    std::cout << "\n--- End of main ---\n";

    return 0;
}
