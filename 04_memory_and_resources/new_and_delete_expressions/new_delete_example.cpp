#include <iostream>
#include <new>
#include <memory>
#include <vector>
#include <numeric>
#include <algorithm>
#include <span>
#include <cstdlib>
#include <string_view>
#include <format>
#include <array>
#include <cassert>
#include <cstring>

class MyClass {
    int value_;

public:
    MyClass() : value_(0) {
        std::cout << std::format("MyClass() : value=0\n");
    }

    explicit MyClass(int v) : value_(v) {
        std::cout << std::format("MyClass({})\n", v);
    }

    ~MyClass() {
        std::cout << std::format("~MyClass({})\n", value_);
    }

    MyClass(const MyClass&)            = default;
    MyClass& operator=(const MyClass&) = default;
    MyClass(MyClass&&)                 = default;
    MyClass& operator=(MyClass&&)      = default;

    [[nodiscard]] int value() const noexcept { return value_; }

    void show() const {
        std::cout << std::format("value = {}\n", value_);
    }

    static void* operator new(std::size_t size) {
        std::cout << std::format("operator new({} bytes)\n", size);
        return ::operator new(size);
    }

    static void* operator new(std::size_t size, const std::nothrow_t&) noexcept {
        std::cout << std::format("operator new nothrow({} bytes)\n", size);
        return ::operator new(size, std::nothrow);
    }

    static void* operator new(std::size_t size, void* place) noexcept {
        std::cout << std::format("operator new placement({} bytes)\n", size);
        return ::operator new(size, place);
    }

    static void* operator new[](std::size_t size) {
        std::cout << std::format("operator new[]({} bytes)\n", size);
        return ::operator new(size);
    }

    static void* operator new[](std::size_t size, void* place) noexcept {
        std::cout << std::format("operator new[] placement({} bytes)\n", size);
        return ::operator new(size, place);
    }

    static void operator delete(void* ptr) noexcept {
        std::cout << "operator delete\n";
        ::operator delete(ptr);
    }

    static void operator delete(void* ptr, std::size_t size) noexcept {
        std::cout << std::format("sized operator delete({} bytes)\n", size);
        ::operator delete(ptr, size);
    }

    static void operator delete(void* ptr, const std::nothrow_t&) noexcept {
        std::cout << "operator delete nothrow\n";
        ::operator delete(ptr);
    }

    static void operator delete(void*, void*) noexcept {
        std::cout << "placement operator delete\n";
    }

    static void operator delete[](void* ptr) noexcept {
        std::cout << "operator delete[]\n";
        ::operator delete(ptr);
    }

    static void operator delete[](void*, void*) noexcept {
        std::cout << "placement operator delete[]\n";
    }
};

class ScopeTracker {
    std::string_view label_;
public:
    explicit ScopeTracker(std::string_view label) : label_(label) {
        std::cout << std::format("[Entering Scope: {}]\n", label_);
    }
    ~ScopeTracker() {
        std::cout << std::format("[Leaving Scope: {}]\n", label_);
    }
    ScopeTracker(const ScopeTracker&)            = delete;
    ScopeTracker& operator=(const ScopeTracker&) = delete;
};

template <std::size_t Capacity>
class ArenaAllocator {
    alignas(std::max_align_t) std::array<std::byte, Capacity> buf_{};
    std::size_t offset_{0};

public:
    [[nodiscard]] void* allocate(std::size_t size, std::size_t align = alignof(std::max_align_t)) {
        const std::size_t aligned = (offset_ + align - 1) & ~(align - 1);
        if (aligned + size > Capacity) throw std::bad_alloc{};
        void* ptr = buf_.data() + aligned;
        offset_ = aligned + size;
        std::cout << std::format("[Arena] allocate {} bytes at offset {}\n", size, aligned);
        return ptr;
    }

    void reset() noexcept {
        std::cout << "[Arena] reset\n";
        offset_ = 0;
    }

    [[nodiscard]] std::size_t used()      const noexcept { return offset_; }
    [[nodiscard]] std::size_t remaining() const noexcept { return Capacity - offset_; }
};

[[nodiscard]] std::unique_ptr<MyClass> createObject(int value) {
    std::cout << std::format("[factory] creating MyClass({})\n", value);
    return std::make_unique<MyClass>(value);
}

void placementNewDemo() {
    std::cout << "\n--- Placement New ---\n";
    alignas(MyClass) std::byte storage[sizeof(MyClass)];
    auto* p = new(storage) MyClass(99);
    p->show();
    std::destroy_at(p);
}

void constructAtDemo() {
    std::cout << "\n--- std::construct_at / std::destroy_at (C++20) ---\n";
    alignas(MyClass) std::byte storage[sizeof(MyClass)];
    auto* p = reinterpret_cast<MyClass*>(&storage);
    std::construct_at(p, 555);
    p->show();
    std::destroy_at(p);
}

void manualArrayPlacementDemo() {
    std::cout << "\n--- Manual Array Placement ---\n";
    constexpr std::size_t count = 2;
    alignas(MyClass) std::byte storage[sizeof(MyClass) * count];
    auto* arr = reinterpret_cast<MyClass*>(storage);
    std::construct_at(&arr[0], 10);
    std::construct_at(&arr[1], 20);
    std::span<MyClass> view(arr, count);
    for (const auto& obj : view) obj.show();
    std::destroy(view.begin(), view.end());
}

void nothrowDemo() {
    std::cout << "\n--- nothrow new ---\n";
    auto* safe = new(std::nothrow) MyClass(77);
    if (safe) { safe->show(); delete safe; }
    else       { std::cout << "allocation failed\n"; }
}

void nullptrDeleteDemo() {
    std::cout << "\n--- Deleting nullptr ---\n";
    MyClass* nullObj = nullptr;
    delete nullObj;
    std::cout << "delete nullptr: no-op\n";
}

void smartPointerDemo() {
    std::cout << "\n--- Smart Pointer Demo ---\n";
    auto uptr = std::make_unique<MyClass>(500);
    uptr->show();

    auto shared = std::make_shared<MyClass>(600);
    std::weak_ptr<MyClass> weak = shared;
    if (auto locked = weak.lock()) {
        locked->show();
        std::cout << std::format("use_count={}\n", shared.use_count());
    }
    shared.reset();
    std::cout << std::format("weak expired after reset: {}\n", weak.expired());
}

void vectorDemo() {
    std::cout << "\n--- Vector of unique_ptr Demo ---\n";
    std::vector<std::unique_ptr<MyClass>> objects;
    objects.reserve(3);
    for (int i = 0; i < 3; ++i)
        objects.push_back(std::make_unique<MyClass>(i * 100));
    for (const auto& obj : objects) obj->show();
}

void mallocDemo() {
    std::cout << "\n--- malloc/free Demo ---\n";
    auto* ptr = static_cast<int*>(std::malloc(sizeof(int)));
    if (!ptr) { std::cerr << "malloc failed\n"; return; }
    *ptr = 1234;
    std::cout << std::format("malloc value: {}\n", *ptr);
    std::free(ptr);
}

void dynamicArrayDemo() {
    std::cout << "\n--- Dynamic Object Array (std::vector) ---\n";
    std::vector<MyClass> objects;
    objects.reserve(3);
    for (int i = 0; i < 3; ++i) objects.emplace_back(i * 10);
    for (const auto& obj : objects) obj.show();
}

void doublePointerDemo() {
    std::cout << "\n--- Double Pointer Demo ---\n";
    constexpr std::size_t count = 2;
    auto ptrArray = std::make_unique<std::unique_ptr<MyClass>[]>(count);
    ptrArray[0] = std::make_unique<MyClass>(1000);
    ptrArray[1] = std::make_unique<MyClass>(2000);
    for (std::size_t i = 0; i < count; ++i) ptrArray[i]->show();
}

void arenaDemo() {
    std::cout << "\n--- Arena Allocator Demo ---\n";
    ArenaAllocator<512> arena;

    void* slot1 = arena.allocate(sizeof(MyClass), alignof(MyClass));
    auto* obj1  = std::construct_at(static_cast<MyClass*>(slot1), 111);

    void* slot2 = arena.allocate(sizeof(MyClass), alignof(MyClass));
    auto* obj2  = std::construct_at(static_cast<MyClass*>(slot2), 222);

    obj1->show();
    obj2->show();

    std::cout << std::format("Arena used={} remaining={}\n",
                             arena.used(), arena.remaining());

    std::destroy_at(obj2);
    std::destroy_at(obj1);

    arena.reset();
    std::cout << std::format("Arena after reset: used={}\n", arena.used());
}

void launerDemo() {
    std::cout << "\n--- std::launder demo ---\n";
    alignas(MyClass) std::byte storage[sizeof(MyClass)];
    new(storage) MyClass(777);
    auto* q = std::launder(reinterpret_cast<MyClass*>(storage));
    q->show();
    std::destroy_at(q);
}

int main() {
    ScopeTracker mainTracker("main");

    std::cout << "\n--- Single Object ---\n";
    {
        auto obj = std::make_unique<MyClass>(42);
        obj->show();
    }

    std::cout << "\n--- Array of int (std::vector) ---\n";
    {
        std::vector<int> arr(10);
        std::iota(arr.begin(), arr.end(), 0);
        for (const int v : arr) std::cout << v << ' ';
        std::cout << '\n';
    }

    std::cout << "\n--- Array of MyClass (new[]) ---\n";
    {
        MyClass* objs = new MyClass[2]{MyClass(1), MyClass(2)};
        std::span<MyClass> view(objs, 2);
        for (const auto& obj : view) obj.show();
        delete[] objs;
    }

    placementNewDemo();
    constructAtDemo();
    nothrowDemo();
    manualArrayPlacementDemo();
    nullptrDeleteDemo();

    std::cout << "\n--- Factory Function ---\n";
    {
        auto factoryObj = createObject(300);
        factoryObj->show();
    }

    smartPointerDemo();
    vectorDemo();
    mallocDemo();
    dynamicArrayDemo();
    doublePointerDemo();
    arenaDemo();
    launerDemo();

    return 0;
}
