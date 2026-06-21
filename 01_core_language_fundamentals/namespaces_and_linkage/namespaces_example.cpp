#include <iostream>
#include <vector>
#include <string_view>
#include <algorithm>
#include <ranges>
#include <utility>
#include <compare>
#include <span>

namespace my_lib {

class MyClass {
    int   value_;
    inline static int object_count_ = 0;

public:
    MyClass() noexcept : value_(0) {
        ++object_count_;
        std::cout << "MyClass()\n";
    }

    explicit MyClass(int v) noexcept : value_(v) {
        ++object_count_;
        std::cout << "MyClass(" << v << ")\n";
    }

    MyClass(const MyClass& other) noexcept : value_(other.value_) {
        ++object_count_;
        std::cout << "MyClass(copy)\n";
    }

    MyClass(MyClass&& other) noexcept : value_(std::exchange(other.value_, 0)) {
        ++object_count_;
        std::cout << "MyClass(move)\n";
    }

    ~MyClass() {
        --object_count_;
        std::cout << "~MyClass(" << value_ << ")\n";
    }

    MyClass& operator=(const MyClass& other) noexcept {
        if (this != &other) value_ = other.value_;
        std::cout << "operator=(copy)\n";
        return *this;
    }

    MyClass& operator=(MyClass&& other) noexcept {
        if (this != &other) { value_ = std::exchange(other.value_, 0); }
        std::cout << "operator=(move)\n";
        return *this;
    }

    [[nodiscard]] auto operator<=>(const MyClass&) const noexcept = default;

    [[nodiscard]] MyClass operator+(const MyClass& rhs) const noexcept {
        return MyClass(value_ + rhs.value_);
    }

    MyClass& operator+=(const MyClass& rhs) noexcept {
        value_ += rhs.value_;
        return *this;
    }

    MyClass& operator++() noexcept { ++value_; return *this; }

    [[nodiscard]] int  value()        const noexcept { return value_; }
    void               set_value(int v)    noexcept { value_ = v; }
    void               reset()             noexcept { value_ = 0; }

    void display() const {
        std::cout << "value = " << value_ << '\n';
    }

    [[nodiscard]] static int object_count() noexcept { return object_count_; }
};

} // namespace my_lib

std::ostream& operator<<(std::ostream& os, const my_lib::MyClass& obj) {
    return os << "MyClass(" << obj.value() << ')';
}

void show_copy(const my_lib::MyClass& obj) {
    my_lib::MyClass copy = obj;
    std::cout << "Copied: " << copy << '\n';
}

void show_pass_by_value(my_lib::MyClass obj) {
    std::cout << "By value: " << obj << '\n';
}

void show_collection(std::span<const my_lib::MyClass> objects) {
    std::cout << "\nCollection:\n";
    std::ranges::for_each(objects, [](const auto& o) { std::cout << o << '\n'; });
}

[[nodiscard]] my_lib::MyClass create_object(int value) {
    return my_lib::MyClass(value);
}

void print_divider() { std::cout << "-----------------------------\n"; }

int main() {
    my_lib::MyClass obj;
    my_lib::MyClass obj2(50);
    my_lib::MyClass obj3(77);

    std::cout << obj  << '\n';
    std::cout << obj2 << '\n';
    std::cout << obj3 << '\n';

    std::cout << "\n--- Comparison ---\n";
    std::cout << "obj == obj2 ? " << (obj == obj2 ? "Yes" : "No") << '\n';
    std::cout << "obj < obj3 ?  " << (obj <  obj3 ? "Yes" : "No") << '\n';
    std::cout << "obj2 != obj3? " << (obj2 != obj3 ? "Yes" : "No") << '\n';

    show_copy(obj3);

    std::cout << "\n--- Assignment ---\n";
    obj = obj2;
    std::cout << obj << '\n';

    std::cout << "\n--- Addition ---\n";
    my_lib::MyClass sum = obj2 + obj3;
    std::cout << sum << '\n';

    std::cout << "\n--- += ---\n";
    obj2 += obj3;
    std::cout << obj2 << '\n';

    std::cout << "\n--- Increment ---\n";
    ++obj3;
    std::cout << obj3 << '\n';

    std::cout << "\n--- set_value ---\n";
    obj.set_value(99);
    std::cout << obj << '\n';

    show_pass_by_value(obj2);

    std::vector<my_lib::MyClass> objects;
    objects.reserve(3);
    objects.push_back(obj);
    objects.push_back(obj2);
    objects.push_back(obj3);
    show_collection(objects);

    std::cout << "\n--- Factory + Move ---\n";
    auto generated = create_object(500);
    std::cout << generated << '\n';
    auto moved = std::move(generated);
    std::cout << moved << '\n';

    print_divider();

    std::cout << "Reset:\n";
    obj.reset();
    std::cout << obj << '\n';

    obj3.display();

    std::cout << "\n--- Sorting ---\n";
    std::ranges::sort(objects);
    show_collection(objects);

    print_divider();
    std::cout << "Object count: " << my_lib::MyClass::object_count() << '\n';

    return 0;
}
