#include <iostream>
#include <array>
#include <span>

void print_value(const int& val) {
    std::cout << "Value (const ref): " << val << "\n";
}

class Demo {
public:
    int data;
    mutable int access_count = 0;

    explicit Demo(int d) : data{d} {}

    [[nodiscard]] int getData() const {
        ++access_count;
        return data;
    }
};

void modify_via_const_cast(const int* ptr) {
    *const_cast<int*>(ptr) = 999;
}

constexpr int square(int x) noexcept {
    return x * x;
}

void volatile_demo() {
    volatile int val = 10;
    const volatile int* ptr = &val;
    std::cout << "Volatile value via pointer: " << *ptr << "\n";
}

void show(int& x) {
    std::cout << "Non-const reference: " << x << "\n";
}

void show(const int& x) {
    std::cout << "Const reference: " << x << "\n";
}

[[nodiscard]] const int& getConstRef(const int& x) noexcept {
    return x;
}

void pointer_const_demo() {
    int a = 10;
    int b = 20;

    int*             p1 = &a;
    const int*       p2 = &a;
    int* const       p3 = &a;
    const int* const p4 = &a;

    p1 = &b;
    p2 = &b;
    *p3 = 40;

    std::cout << "p1 → " << *p1 << "\n"
              << "p2 → " << *p2 << "\n"
              << "p3 → " << *p3 << "\n"
              << "p4 → " << *p4 << "\n";
}

class ConstexprDemo {
public:
    int val;

    constexpr explicit ConstexprDemo(int v) noexcept : val{v} {}

    [[nodiscard]] constexpr int get() const noexcept {
        return val;
    }
};

void print_array_const(std::span<const int> arr) {
    std::cout << "Const array values: ";
    for (const int v : arr) {
        std::cout << v << " ";
    }
    std::cout << "\n";
}

void const_local_demo() {
    constexpr int fixed = 500;
    std::cout << "Const local value: " << fixed << "\n";
}

void pointer_reader(const int* ptr) {
    std::cout << "Pointer reader value: " << *ptr << "\n";
}

int main() {
    int x = 10;
    constexpr int a = 5;

    int* const ptr1 = &x;
    *ptr1 = 15;

    const int* ptr2 = &x;
    ptr2 = &a;

    [[maybe_unused]] const int* const ptr3 = &x;

    volatile int v = 100;
    v = 200;

    constexpr volatile int cv = 300;

    std::cout << "x:  " << x  << "\n"
              << "a:  " << a  << "\n"
              << "v:  " << v  << "\n"
              << "cv: " << cv << "\n";

    std::cout << "\n--- print_value ---\n";
    print_value(x);

    std::cout << "\n--- mutable member ---\n";
    Demo d{50};
    std::cout << "Demo data: " << d.getData() << "\n"
              << "Access count: " << d.access_count << "\n";

    std::cout << "\n--- const_cast modify ---\n";
    int normal = 123;
    modify_via_const_cast(&normal);
    std::cout << "After const_cast modify: " << normal << "\n";

    std::cout << "\n--- constexpr square ---\n";
    constexpr int sq = square(5);
    std::cout << "Square (compile-time): " << sq << "\n";

    std::cout << "\n--- volatile pointer ---\n";
    volatile_demo();

    std::cout << "\n--- const overload ---\n";
    show(x);
    show(a);

    std::cout << "\n--- const reference return ---\n";
    const int& ref = getConstRef(x);
    std::cout << "Const ref value: " << ref << "\n";

    std::cout << "\n--- pointer const variations ---\n";
    pointer_const_demo();

    std::cout << "\n--- constexpr object ---\n";
    constexpr ConstexprDemo obj{77};
    std::cout << "Constexpr object value: " << obj.get() << "\n";

    std::cout << "\n--- const array (std::span) ---\n";
    constexpr std::array numbers{1, 2, 3, 4, 5};
    print_array_const(numbers);

    std::cout << "\n--- const local ---\n";
    const_local_demo();

    std::cout << "\n--- const pointer parameter ---\n";
    pointer_reader(&x);

    return 0;
}
