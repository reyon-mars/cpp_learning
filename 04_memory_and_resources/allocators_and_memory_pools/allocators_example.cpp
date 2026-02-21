// Allocators and Memory Pools Exercise
// Custom allocation strategies

#include <iostream>
#include <memory>
#include <vector>

template<typename T>
class SimpleAllocator {
public:
    using value_type = T;
    
    SimpleAllocator() = default;
    
    template<typename U>
    SimpleAllocator(const SimpleAllocator<U>&) {}
    
    T* allocate(std::size_t n) {
        std::cout << "Allocating " << n << " objects\n";
        return new T[n];
    }
    
    void deallocate(T* p, std::size_t n) {
        std::cout << "Deallocating " << n << " objects\n";
        delete[] p;
    }
};

int main() {
    std::vector<int, SimpleAllocator<int>> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    
    return 0;
}
