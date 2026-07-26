#include <array>
#include <iostream>
#include <memory>
#include <span>
#include <utility>
#include <vector>

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

class ExpensiveLookup {
public:
    explicit ExpensiveLookup(int seed) : seed_{seed} {}
    [[nodiscard]] int cached_value() const {
        if (!cache_valid_) {
            std::cout << "  (computing expensive value, caching it)\n";
            cache_ = seed_ * seed_ + 1;
            cache_valid_ = true;
        } else {
            std::cout << "  (returning cached value)\n";
        }
        return cache_;
    }

private:
    int seed_;
    mutable int cache_ = 0;
    mutable bool cache_valid_ = false;
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

class Row {
public:
    explicit Row(std::size_t n) : values_(n, 0) {}

    [[nodiscard]] const int& at(std::size_t i) const {
        return values_.at(i);
    }
    [[nodiscard]] int& at(std::size_t i) {
        return const_cast<int&>(std::as_const(*this).at(i));
    }

private:
    std::vector<int> values_;
};

void non_duplicated_const_overload_demo() {
    Row row{3};
    row.at(0) = 111;
    const Row& const_row = row;
    std::cout << "Row.at(0) via non-const overload set to 111\n"
              << "Row.at(0) via const overload reads: " << const_row.at(0) << "\n";
}

void smart_pointer_constness_demo() {
    const std::unique_ptr<int> fixed_owner = std::make_unique<int>(1);
    *fixed_owner = 2;
    std::cout << "const unique_ptr<int>: pointee mutable, now=" << *fixed_owner << "\n";

    std::unique_ptr<const int> immutable_pointee = std::make_unique<int>(3);
    std::cout << "unique_ptr<const int>: pointee immutable, value=" << *immutable_pointee << "\n";
    immutable_pointee = std::make_unique<const int>(4);
    std::cout << "unique_ptr<const int>: pointer itself reassignable, now points to=" << *immutable_pointee << "\n";
}

void as_const_demo() {
    std::vector<int> mutable_vec{1, 2, 3};
    for (const int& v : std::as_const(mutable_vec)) {
        std::cout << "as_const element: " << v << "\n";
    }
}

constinit int program_start_flag = 1;

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
    std::cout << "x:   " << x   << "\n"
              << "a:   " << a   << "\n"
              << "v:   " << v   << "\n"
              << "cv:  " << cv  << "\n"
              << "ptr2 (after reassign to &a): " << *ptr2 << "\n"
              << "program_start_flag (constinit): " << program_start_flag << "\n";

    std::cout << "\n--- print_value ---\n";
    print_value(x);

    std::cout << "\n--- mutable member (access counter) ---\n";
    Demo d{50};
    std::cout << "Demo data: " << d.getData() << "\n"
              << "Access count: " << d.access_count << "\n";

    std::cout << "\n--- mutable member (logical constness / caching) ---\n";
    const ExpensiveLookup lookup{7};
    std::cout << "first call:  " << lookup.cached_value() << "\n";
    std::cout << "second call: " << lookup.cached_value() << "\n";

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

    std::cout << "\n--- avoiding const/non-const overload duplication ---\n";
    non_duplicated_const_overload_demo();

    std::cout << "\n--- const unique_ptr<T> vs unique_ptr<const T> ---\n";
    smart_pointer_constness_demo();

    std::cout << "\n--- std::as_const ---\n";
    as_const_demo();

    return 0;
}
