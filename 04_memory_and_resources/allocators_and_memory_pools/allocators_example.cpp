#include <iostream>
#include <memory>
#include <vector>
#include <new>
#include <cstddef>
#include <type_traits>
#include <cassert>
#include <algorithm>
#include <span>

template<typename T>
class SimpleAllocator {
public:
    using value_type      = T;
    using size_type       = std::size_t;
    using difference_type = std::ptrdiff_t;
    using propagate_on_container_move_assignment = std::true_type;
    using is_always_equal = std::true_type;

    SimpleAllocator() noexcept = default;

    template<typename U>
    SimpleAllocator(const SimpleAllocator<U>&) noexcept {}

    [[nodiscard]] T* allocate(size_type n) {
        std::cout << "[Allocator] allocate " << n
                  << " object(s) (" << n * sizeof(T) << " bytes)\n";
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }

    void deallocate(T* p, size_type n) noexcept {
        std::cout << "[Allocator] deallocate " << n << " object(s)\n";
        ::operator delete(p);
    }

    [[nodiscard]] size_type max_size() const noexcept {
        return size_type(-1) / sizeof(T);
    }

    [[nodiscard]] bool operator==(const SimpleAllocator&) const noexcept { return true;  }
    [[nodiscard]] bool operator!=(const SimpleAllocator&) const noexcept { return false; }
};

class IntPool {
public:
    explicit IntPool(std::size_t size) : pool_(size) {}

    [[nodiscard]] int* allocate() {
        if (index_ >= pool_.size()) throw std::bad_alloc();
        std::cout << "[IntPool] allocate slot=" << index_ << "\n";
        return &pool_[index_++];
    }

    void reset() noexcept {
        std::cout << "[IntPool] reset\n";
        index_ = 0;
    }

    [[nodiscard]] std::size_t capacity() const noexcept { return pool_.size(); }
    [[nodiscard]] std::size_t used()     const noexcept { return index_;       }
    [[nodiscard]] bool        full()     const noexcept { return index_ >= pool_.size(); }

private:
    std::vector<int> pool_;
    std::size_t      index_ = 0;
};

struct Test {
    int x;
    explicit Test(int v) : x{v} { std::cout << "Test constructed x=" << x << "\n"; }
    ~Test()                      { std::cout << "Test destroyed   x=" << x << "\n"; }
};

template<typename T>
class ObjectPool {
public:
    explicit ObjectPool(std::size_t size) : index_{0} {
        buffer_.reserve(size);
        buffer_.resize(size);
    }

    template<typename... Args>
    [[nodiscard]] T* create(Args&&... args) {
        if (index_ >= buffer_.size()) throw std::bad_alloc();
        void* place = &buffer_[index_];
        std::cout << "[ObjectPool] create slot=" << index_ << "\n";
        ++index_;
        return ::new (place) T(std::forward<Args>(args)...);
    }

    void destroyAll() noexcept {
        for (std::size_t i = 0; i < index_; ++i) {
            std::launder(reinterpret_cast<T*>(&buffer_[i]))->~T();
        }
        index_ = 0;
        std::cout << "[ObjectPool] destroyAll + reset\n";
    }

    [[nodiscard]] std::size_t capacity() const noexcept { return buffer_.size();        }
    [[nodiscard]] std::size_t used()     const noexcept { return index_;                }
    [[nodiscard]] bool        full()     const noexcept { return index_ >= buffer_.size(); }

private:
    using Storage = std::aligned_storage_t<sizeof(T), alignof(T)>;
    std::vector<Storage> buffer_;
    std::size_t          index_;
};

class ScopeGuard {
public:
    ScopeGuard()  { std::cout << "[ScopeGuard] enter\n"; }
    ~ScopeGuard() { std::cout << "[ScopeGuard] exit\n";  }
};

void printVector(std::span<const int> vec) {
    std::cout << "Vector: ";
    for (int v : vec) { std::cout << v << " "; }
    std::cout << "\n";
}

void printPoolState(const IntPool& pool) {
    std::cout << "IntPool usage=" << pool.used() << "/" << pool.capacity()
              << " full=" << std::boolalpha << pool.full() << "\n";
}

void placementNewDemo() {
    std::cout << "\n--- Placement new ---\n";
    alignas(Test) std::byte raw[sizeof(Test)];
    auto* obj = ::new (raw) Test{555};
    std::cout << "Placement value=" << obj->x << "\n";
    std::destroy_at(obj);
}

template<typename T>
void showAlignment() {
    std::cout << "sizeof=" << sizeof(T) << " alignof=" << alignof(T) << "\n";
}

void allocatorVectorDemo() {
    std::cout << "\n--- Custom allocator vector ---\n";
    std::vector<Test, SimpleAllocator<Test>> objects;
    objects.reserve(3);
    objects.emplace_back(1);
    objects.emplace_back(2);
    objects.emplace_back(3);
    std::cout << "size=" << objects.size() << "\n";
}

int main() {
    std::cout << "\n--- SimpleAllocator vector ---\n";
    std::vector<int, SimpleAllocator<int>> vec;
    vec.reserve(4);
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    printVector(vec);
    assert(vec.size() == 3);

    std::cout << "\n--- IntPool ---\n";
    IntPool pool{3};
    int* a = pool.allocate();
    int* b = pool.allocate();
    int* c = pool.allocate();
    *a = 10; *b = 20; *c = 30;
    std::cout << "Values: " << *a << " " << *b << " " << *c << "\n";
    printPoolState(pool);
    pool.reset();

    std::cout << "\n--- allocator_traits construct/destroy ---\n";
    SimpleAllocator<Test> alloc;
    Test* t = alloc.allocate(1);
    std::allocator_traits<SimpleAllocator<Test>>::construct(alloc, t, 42);
    std::cout << "Test value=" << t->x << "\n";
    std::allocator_traits<SimpleAllocator<Test>>::destroy(alloc, t);
    alloc.deallocate(t, 1);

    std::cout << "\n--- ObjectPool ---\n";
    {
        ScopeGuard guard;
        ObjectPool<Test> objPool{2};
        Test* p1 = objPool.create(100);
        Test* p2 = objPool.create(200);
        std::cout << "Objects: " << p1->x << ", " << p2->x << "\n";
        std::cout << "usage=" << objPool.used() << "/" << objPool.capacity()
                  << " full=" << objPool.full() << "\n";
        objPool.destroyAll();
    }

    placementNewDemo();

    std::cout << "\n--- Alignment ---\n";
    showAlignment<Test>();

    std::cout << "\n--- Type traits ---\n";
    std::cout << "trivially_destructible=" << std::boolalpha
              << std::is_trivially_destructible_v<Test> << "\n"
              << "move_constructible="
              << std::is_move_constructible_v<Test> << "\n";

    allocatorVectorDemo();

    std::cout << "\n--- End of main ---\n";
    return 0;
}
