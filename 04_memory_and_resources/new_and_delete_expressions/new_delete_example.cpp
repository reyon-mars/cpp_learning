#include <iostream>
#include <new>
#include <memory>
#include <vector>
#include <numeric>
#include <algorithm>
#include <span>
#include <cstdlib>
#include <string_view>

class MyClass {
    int value_;

public:
    MyClass() : value_(0) {
        std::cout << "MyClass() : value=0\n";
    }

    explicit MyClass(int v) : value_(v) {
        std::cout << "MyClass(" << v << ")\n";
    }

    ~MyClass() {
        std::cout << "~MyClass(" << value_ << ")\n";
    }

    MyClass(const MyClass&)            = default;
    MyClass& operator=(const MyClass&) = default;
    MyClass(MyClass&&)                 = default;
    MyClass& operator=(MyClass&&)      = default;

    [[nodiscard]] int value() const noexcept { return value_; }

    void show() const {
        std::cout << "value = " << value_ << '\n';
    }

    static void* operator new(std::size_t size) {
        std::cout << "operator new(" << size << " bytes)\n";
        return ::operator new(size);
    }

    static void* operator new(std::size_t size, const std::nothrow_t&) noexcept {
        std::cout << "operator new nothrow(" << size << " bytes)\n";
        return ::operator new(size, std::nothrow);
    }

    static void* operator new(std::size_t size, void* place) noexcept {
        std::cout << "operator new placement(" << size << " bytes)\n";
        return ::operator new(size, place);
    }

    static void* operator new[](std::size_t size) {
        std::cout << "operator new[](" << size << " bytes)\n";
        return ::operator new(size);
    }

    static void* operator new[](std::size_t size, void* place) noexcept {
        std::cout << "operator new[] placement(" << size << " bytes)\n";
        return ::operator new(size, place);
    }

    static void operator delete(void* ptr) noexcept {
        std::cout << "operator delete\n";
        ::operator delete(ptr);
    }

    static void operator delete(void* ptr, std::size_t size) noexcept {
        std::cout << "sized operator delete(" << size << " bytes)\n";
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
        std::cout << "[Entering Scope: " << label_ << "]\n";
    }
    ~ScopeTracker() {
        std::cout << "[Leaving Scope: " << label_ << "]\n";
    }
    ScopeTracker(const ScopeTracker&)            = delete;
    ScopeTracker& operator=(const ScopeTracker&) = delete;
};

[[nodiscard]] std::unique_ptr<MyClass> createObject(int value) {
    std::cout << "[factory] creating MyClass(" << value << ")\n";
    return std::make_unique<MyClass>(value);
}

void placementNewDemo() {
    std::cout << "\n--- Placement New ---\n";
    alignas(MyClass) std::byte storage[sizeof(MyClass)];
    auto* p = new(storage) MyClass(99);
    p->show();
    p->~MyClass();
}

void manualArrayPlacementDemo() {
    std::cout << "\n--- Manual Array Placement ---\n";
    constexpr std::size_t count = 2;
    alignas(MyClass) std::byte storage[sizeof(MyClass) * count];
    auto* arr = reinterpret_cast<MyClass*>(storage);
    new(&arr[0]) MyClass(10);
    new(&arr[1]) MyClass(20);
    std::span<MyClass> view(arr, count);
    for (const auto& obj : view) {
        obj.show();
    }
    std::for_each(view.rbegin(), view.rend(), [](MyClass& obj) { obj.~MyClass(); });
}

void nothrowDemo() {
    std::cout << "\n--- nothrow new ---\n";
    auto* safe = new(std::nothrow) MyClass(77);
    if (safe) {
        safe->show();
        delete safe;
    } else {
        std::cout << "allocation failed\n";
    }
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
    }
}

void vectorDemo() {
    std::cout << "\n--- Vector of unique_ptr Demo ---\n";
    std::vector<std::unique_ptr<MyClass>> objects;
    objects.reserve(3);
    for (int i = 0; i < 3; ++i) {
        objects.push_back(std::make_unique<MyClass>(i * 100));
    }
    for (const auto& obj : objects) {
        obj->show();
    }
}

void mallocDemo() {
    std::cout << "\n--- malloc/free Demo ---\n";
    auto* ptr = static_cast<int*>(std::malloc(sizeof(int)));
    if (!ptr) {
        std::cerr << "malloc failed\n";
        return;
    }
    *ptr = 1234;
    std::cout << "malloc value: " << *ptr << '\n';
    std::free(ptr);
}

void dynamicArrayDemo() {
    std::cout << "\n--- Dynamic Object Array (std::vector) ---\n";
    std::vector<MyClass> objects;
    objects.reserve(3);
    for (int i = 0; i < 3; ++i) {
        objects.emplace_back(i * 10);
    }
    for (const auto& obj : objects) {
        obj.show();
    }
}

void doublePointerDemo() {
    std::cout << "\n--- Double Pointer Demo ---\n";
    constexpr std::size_t count = 2;
    auto ptrArray = std::make_unique<std::unique_ptr<MyClass>[]>(count);
    ptrArray[0] = std::make_unique<MyClass>(1000);
    ptrArray[1] = std::make_unique<MyClass>(2000);
    for (std::size_t i = 0; i < count; ++i) {
        ptrArray[i]->show();
    }
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
        for (int v : arr) {
            std::cout << v << ' ';
        }
        std::cout << '\n';
    }

    std::cout << "\n--- Array of MyClass (new[]) ---\n";
    {
        MyClass* objs = new MyClass[2]{ MyClass(1), MyClass(2) };
        std::span<MyClass> view(objs, 2);
        for (const auto& obj : view) {
            obj.show();
        }
        delete[] objs;
    }

    placementNewDemo();
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

    return 0;
}
