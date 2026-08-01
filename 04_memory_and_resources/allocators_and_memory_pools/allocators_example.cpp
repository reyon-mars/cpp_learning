#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <format>
#include <iostream>
#include <memory>
#include <memory_resource>
#include <new>
#include <span>
#include <string>
#include <type_traits>
#include <vector>

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

static_assert(std::is_same_v<
    std::allocator_traits<SimpleAllocator<int>>::rebind_alloc<double>,
    SimpleAllocator<double>>);

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

struct LoggedBuffer {
    std::string payload;
    explicit LoggedBuffer(std::string p) : payload{std::move(p)} {
        std::cout << std::format("LoggedBuffer constructed, payload.size()={}\n", payload.size());
    }
    ~LoggedBuffer() {
        std::cout << "LoggedBuffer destroyed\n";
    }
};

void destructor_suppresses_move_demo() {
    std::cout << "\n--- User-declared destructor suppresses the implicit move constructor ---\n";
    std::cout << std::boolalpha
              << "Test:         is_move_constructible=" << std::is_move_constructible_v<Test>
              << " is_nothrow_move_constructible=" << std::is_nothrow_move_constructible_v<Test>
              << " (fine here: Test's only member is a trivial int)\n"
              << "LoggedBuffer: is_move_constructible=" << std::is_move_constructible_v<LoggedBuffer>
              << " is_nothrow_move_constructible=" << std::is_nothrow_move_constructible_v<LoggedBuffer>
              << " (NOT nothrow: no real move ctor exists, so std::move silently falls back to a full copy)\n";

    LoggedBuffer a{std::string(64, 'x')};
    LoggedBuffer b = std::move(a);
    std::cout << "after std::move(a), a.payload is still full (a genuine move ctor would have emptied it): "
              << "a.payload.size()=" << a.payload.size() << ", b.payload.size()=" << b.payload.size() << "\n";
}

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

void pool_exhaustion_demo() {
    std::cout << "\n--- IntPool exhaustion ---\n";
    IntPool small_pool{2};
    [[maybe_unused]] int* first = small_pool.allocate();
    [[maybe_unused]] int* second = small_pool.allocate();
    print_pool_state(small_pool);
    try {
        [[maybe_unused]] int* third = small_pool.allocate();
    } catch (const std::bad_alloc& e) {
        std::cout << "Caught expected bad_alloc on exhausted IntPool: " << e.what() << "\n";
    }

    std::cout << "\n--- ObjectPool exhaustion ---\n";
    ObjectPool<Test> small_object_pool{1};
    Test* only_slot = small_object_pool.create(999);
    std::cout << std::format("Occupied only slot with x={}\n", only_slot->x);
    try {
        [[maybe_unused]] Test* overflow = small_object_pool.create(1000);
    } catch (const std::bad_alloc& e) {
        std::cout << "Caught expected bad_alloc on exhausted ObjectPool: " << e.what() << "\n";
    }
    small_object_pool.destroy_all();
}

void pmr_demo() {
    std::cout << "\n--- std::pmr as the modern alternative to a hand-rolled allocator ---\n";
    std::array<std::byte, 256> stack_buffer{};
    std::pmr::monotonic_buffer_resource resource{stack_buffer.data(), stack_buffer.size()};
    std::pmr::vector<int> pmr_vec{&resource};
    pmr_vec.reserve(4);
    for (int v : {10, 20, 30, 40}) pmr_vec.push_back(v);
    std::cout << "pmr::vector backed by a stack buffer, no heap allocation for small sizes: ";
    for (int v : pmr_vec) std::cout << v << ' ';
    std::cout << "\n";
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
    std::cout << std::format("max_size()={}\n", alloc.max_size());
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

    destructor_suppresses_move_demo();

    allocator_vector_demo();

    pool_exhaustion_demo();

    pmr_demo();

    std::cout << "\n--- End of main ---\n";
    return 0;
}
