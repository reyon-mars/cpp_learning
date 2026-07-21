#include <algorithm>
#include <iostream>
#include <span>
#include <utility>
#include <vector>
#include <numeric>
#include <ranges>
#include <cassert>
#include <functional>
#include <optional>
#include <format>

namespace my_lib {

class MyClass {
    int value_;
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
        if (this != &other) value_ = std::exchange(other.value_, 0);
        std::cout << "operator=(move)\n";
        return *this;
    }

    [[nodiscard]] auto operator<=>(const MyClass&) const noexcept = default;

    [[nodiscard]] MyClass operator+(const MyClass& rhs) const noexcept {
        return MyClass(value_ + rhs.value_);
    }

    [[nodiscard]] MyClass operator-(const MyClass& rhs) const noexcept {
        return MyClass(value_ - rhs.value_);
    }

    [[nodiscard]] MyClass operator*(const MyClass& rhs) const noexcept {
        return MyClass(value_ * rhs.value_);
    }

    MyClass& operator+=(const MyClass& rhs) noexcept { value_ += rhs.value_; return *this; }
    MyClass& operator-=(const MyClass& rhs) noexcept { value_ -= rhs.value_; return *this; }

    MyClass& operator++()    noexcept { ++value_; return *this; }
    MyClass& operator--()    noexcept { --value_; return *this; }

    [[nodiscard]] MyClass operator++(int) noexcept { MyClass tmp = *this; ++value_; return tmp; }
    [[nodiscard]] MyClass operator--(int) noexcept { MyClass tmp = *this; --value_; return tmp; }

    [[nodiscard]] int  value()        const noexcept { return value_; }
    [[nodiscard]] bool is_zero()      const noexcept { return value_ == 0; }
    [[nodiscard]] bool is_positive()  const noexcept { return value_ > 0; }
    [[nodiscard]] bool is_negative()  const noexcept { return value_ < 0; }

    void set_value(int v) noexcept { value_ = v; }
    void reset()          noexcept { value_ = 0; }
    void negate()         noexcept { value_ = -value_; }

    void display() const { std::cout << "value = " << value_ << '\n'; }

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

void show_collection(std::span<const my_lib::MyClass> objects, std::string_view label = "") {
    if (!label.empty()) std::cout << label << '\n';
    std::ranges::for_each(objects, [](const auto& o) { std::cout << o << '\n'; });
}

[[nodiscard]] my_lib::MyClass create_object(int value) {
    return my_lib::MyClass(value);
}

void print_divider() { std::cout << "-----------------------------\n"; }

[[nodiscard]] int sum_collection(std::span<const my_lib::MyClass> objects) noexcept {
    return std::accumulate(objects.begin(), objects.end(), 0,
        [](int acc, const my_lib::MyClass& o) { return acc + o.value(); });
}

[[nodiscard]] std::optional<my_lib::MyClass> max_in_collection(
        std::span<const my_lib::MyClass> objects) {
    if (objects.empty()) return std::nullopt;
    return *std::ranges::max_element(objects);
}

[[nodiscard]] std::optional<my_lib::MyClass> min_in_collection(
        std::span<const my_lib::MyClass> objects) {
    if (objects.empty()) return std::nullopt;
    return *std::ranges::min_element(objects);
}

[[nodiscard]] std::vector<my_lib::MyClass> filter_positive(
        std::span<const my_lib::MyClass> objects) {
    std::vector<my_lib::MyClass> result;
    std::ranges::copy_if(objects, std::back_inserter(result),
        [](const my_lib::MyClass& o) { return o.is_positive(); });
    return result;
}

int main() {
    my_lib::MyClass obj;
    my_lib::MyClass obj2(50);
    my_lib::MyClass obj3(77);

    std::cout << obj << '\n' << obj2 << '\n' << obj3 << '\n';

    std::cout << "\n--- Comparison ---\n";
    std::cout << "obj == obj2 ? " << (obj == obj2 ? "Yes" : "No") << '\n';
    std::cout << "obj < obj3 ?  " << (obj  < obj3 ? "Yes" : "No") << '\n';
    std::cout << "obj2 != obj3? " << (obj2 != obj3 ? "Yes" : "No") << '\n';

    show_copy(obj3);

    std::cout << "\n--- Assignment ---\n";
    obj = obj2;
    std::cout << obj << '\n';

    std::cout << "\n--- Arithmetic ---\n";
    auto sum = obj2 + obj3;  std::cout << "sum:  " << sum  << '\n';
    auto diff = obj3 - obj2; std::cout << "diff: " << diff << '\n';
    auto prod = obj2 * obj3; std::cout << "prod: " << prod << '\n';

    std::cout << "\n--- Compound assignment ---\n";
    obj2 += obj3; std::cout << "obj2 += obj3: " << obj2 << '\n';
    obj2 -= obj3; std::cout << "obj2 -= obj3: " << obj2 << '\n';

    std::cout << "\n--- Increment / Decrement ---\n";
    ++obj3; std::cout << "++obj3: " << obj3 << '\n';
    --obj3; std::cout << "--obj3: " << obj3 << '\n';
    auto post_inc = obj3++; std::cout << "obj3++ was: " << post_inc << " now: " << obj3 << '\n';
    auto post_dec = obj3--; std::cout << "obj3-- was: " << post_dec << " now: " << obj3 << '\n';

    std::cout << "\n--- State predicates ---\n";
    my_lib::MyClass zero;
    my_lib::MyClass neg(-5);
    std::cout << std::format("zero.is_zero()={}  obj2.is_positive()={}  neg.is_negative()={}\n",
        zero.is_zero(), obj2.is_positive(), neg.is_negative());

    std::cout << "\n--- negate ---\n";
    neg.negate();
    std::cout << "After negate: " << neg << '\n';

    std::cout << "\n--- set_value / reset ---\n";
    obj.set_value(99); std::cout << obj << '\n';
    obj.reset();       std::cout << obj << '\n';

    show_pass_by_value(obj2);

    std::vector<my_lib::MyClass> objects;
    objects.reserve(3);
    objects.push_back(obj2);
    objects.push_back(obj3);
    objects.push_back(neg);
    show_collection(objects, "\nCollection:");

    std::cout << "\n--- Collection utilities ---\n";
    std::cout << "sum="  << sum_collection(objects) << '\n';
    if (auto mx = max_in_collection(objects)) std::cout << "max=" << *mx << '\n';
    if (auto mn = min_in_collection(objects)) std::cout << "min=" << *mn << '\n';

    auto pos = filter_positive(objects);
    show_collection(pos, "Positives:");

    std::cout << "\n--- Factory + Move ---\n";
    auto generated = create_object(500);
    std::cout << generated << '\n';
    auto moved = std::move(generated);
    std::cout << moved << '\n';

    print_divider();
    obj3.display();

    std::cout << "\n--- Sorting ---\n";
    std::ranges::sort(objects);
    show_collection(objects, "Sorted:");
    std::ranges::sort(objects, std::greater<>{});
    show_collection(objects, "Sorted desc:");

    print_divider();
    std::cout << "Object count: " << my_lib::MyClass::object_count() << '\n';

    assert((my_lib::MyClass(3) + my_lib::MyClass(4)).value() == 7);
    assert((my_lib::MyClass(10) - my_lib::MyClass(3)).value() == 7);
    assert((my_lib::MyClass(3) * my_lib::MyClass(4)).value() == 12);
    assert(my_lib::MyClass(0).is_zero());
    assert(my_lib::MyClass(5).is_positive());
    assert(my_lib::MyClass(-3).is_negative());

    std::cout << "\nAll assertions passed.\n";
    return 0;
}
