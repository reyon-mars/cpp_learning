#include <iostream>
#include <memory>
#include <vector>
#include <new>
#include <cstddef>
#include <type_traits>
#include <cassert>
#include <algorithm>
#include <span>
#include <format>

template <typename T>
class SimpleAllocator {
public:
    using value_type      = T;
    using size_type       = std::size_t;
    using difference_type = std::ptrdiff_t;
    using propagate_on_container_move_assignment = std::true_type;
    using is_always_equal = std::true_type;

    SimpleAllocator() noexcept = default;

    template <typename U>
    explicit SimpleAllocator(const SimpleAllocator<U>&) noexcept {}

    [[nodiscard]] T* allocate(size_type n) {
        std::cout << std::format("[Allocator] allocate {} object(s) ({} bytes)\n",
                                 n, n * sizeof(T));
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }

    void deallocate(T* p, size_type n) noexcept {
        std::cout << std::format("[Allocator] deallocate {} object(s)\n", n);
        ::operator delete(p);
    }

    [[nodiscard]] size_type max_size() const noexcept {
        return static_cast<size_type>(-1) / sizeof(T);
    }

    [[nodiscard]] bool operator==(const SimpleAllocator&) const noexcept = default;
};

class IntPool {
public:
    explicit IntPool(std::size_t size) : pool_(size) {}

    [[nodiscard]] int* allocate() {
        if (index_ >= pool_.size()) throw std::bad_alloc();
        std::cout << std::format("[IntPool] allocate slot={}\n", index_);
        return &pool_[index_++];
    }

    void reset() noexcept {
        std::cout << "[IntPool] reset\n";
        index_ = 0;
    }

    [[nodiscard]] std::size_t capacity() const noexcept { return pool_.size(); }
    [[nodiscard]] std::size_t used()     const noexcept { return index_; }
    [[nodiscard]] bool        full()     const noexcept { return index_ >= pool_.size(); }

private:
    std::vector<int> pool_;
    std::size_t      index_{0};
};

struct Test {
    int x;
    explicit Test(int v) noexcept : x{v} {
        std::cout << std::format("Test constructed x={}\n", x);
    }
    ~Test() {
        std::cout << std::format("Test destroyed   x={}\n", x);
    }
};

template <typename T>
class ObjectPool {
public:
    explicit ObjectPool(std::size_t size) : buffer_(size) {}

    template <typename... Args>
    [[nodiscard]] T* create(Args&&... args) {
        if (index_ >= buffer_.size()) throw std::bad_alloc();
        void* place = &buffer_[index_];
        std::cout << std::format("[ObjectPool] create slot={}\n", index_);
        ++index_;
        return ::new (place) T(std::forward<Args>(args)...);
    }

    void destroy_all() noexcept {
        for (std::size_t i = 0; i < index_; ++i)
            std::launder(reinterpret_cast<T*>(&buffer_[i]))->~T();
        index_ = 0;
        std::cout << "[ObjectPool] destroy_all + reset\n";
    }

    [[nodiscard]] std::size_t capacity() const noexcept { return buffer_.size(); }
    [[nodiscard]] std::size_t used()     const noexcept { return index_; }
    [[nodiscard]] bool        full()     const noexcept { return index_ >= buffer_.size(); }

private:
    struct alignas(T) Storage { std::byte data[sizeof(T)]; };
    std::vector<Storage> buffer_;
    std::size_t          index_{0};
};

class ScopeGuard {
public:
    ScopeGuard()  { std::cout << "[ScopeGuard] enter\n"; }
    ~ScopeGuard() { std::cout << "[ScopeGuard] exit\n"; }

    ScopeGuard(const ScopeGuard&)            = delete;
    ScopeGuard& operator=(const ScopeGuard&) = delete;
};

void print_vector(std::span<const int> vec) {
    std::cout << "Vector: ";
    for (const int v : vec) std::cout << v << ' ';
    std::cout << '\n';
}

void print_pool_state(const IntPool& pool) {
    std::cout << std::format("IntPool usage={}/{} full={}\n",
                             pool.used(), pool.capacity(), pool.full());
}

void placement_new_demo() {
    std::cout << "\n--- Placement new ---\n";
    alignas(Test) std::byte raw[sizeof(Test)];
    auto* obj = ::new (raw) Test{555};
    std::cout << std::format("Placement value={}\n", obj->x);
    std::destroy_at(obj);
}

template <typename T>
void show_alignment() {
    std::cout << std::format("sizeof={} alignof={}\n", sizeof(T), alignof(T));
}

void allocator_vector_demo() {
    std::cout << "\n--- Custom allocator vector ---\n";
    std::vector<Test, SimpleAllocator<Test>> objects;
    objects.reserve(3);
    objects.emplace_back(1);
    objects.emplace_back(2);
    objects.emplace_back(3);
    std::cout << std::format("size={}\n", objects.size());
}

int main() {
    std::cout << "\n--- SimpleAllocator vector ---\n";
    std::vector<int, SimpleAllocator<int>> vec;
    vec.reserve(4);
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    print_vector(vec);
    assert(vec.size() == 3);

    std::cout << "\n--- IntPool ---\n";
    IntPool pool{3};
    int* a = pool.allocate();
    int* b = pool.allocate();
    int* c = pool.allocate();
    *a = 10; *b = 20; *c = 30;
    std::cout << std::format("Values: {} {} {}\n", *a, *b, *c);
    print_pool_state(pool);
    pool.reset();

    std::cout << "\n--- allocator_traits construct/destroy ---\n";
    SimpleAllocator<Test> alloc;
    Test* t = alloc.allocate(1);
    std::allocator_traits<SimpleAllocator<Test>>::construct(alloc, t, 42);
    std::cout << std::format("Test value={}\n", t->x);
    std::allocator_traits<SimpleAllocator<Test>>::destroy(alloc, t);
    alloc.deallocate(t, 1);

    std::cout << "\n--- ObjectPool ---\n";
    {
        const ScopeGuard guard;
        ObjectPool<Test> obj_pool{2};
        Test* p1 = obj_pool.create(100);
        Test* p2 = obj_pool.create(200);
        std::cout << std::format("Objects: {}, {}\n", p1->x, p2->x);
        std::cout << std::format("usage={}/{} full={}\n",
                                 obj_pool.used(), obj_pool.capacity(), obj_pool.full());
        obj_pool.destroy_all();
    }

    placement_new_demo();

    std::cout << "\n--- Alignment ---\n";
    show_alignment<Test>();

    std::cout << "\n--- Type traits ---\n";
    std::cout << std::format("trivially_destructible={}\nmove_constructible={}\n",
                             std::is_trivially_destructible_v<Test>,
                             std::is_move_constructible_v<Test>);

    allocator_vector_demo();

    std::cout << "\n--- End of main ---\n";
    return 0;
}
