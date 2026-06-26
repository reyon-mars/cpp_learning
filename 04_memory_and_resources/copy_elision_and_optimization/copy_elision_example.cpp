#include <iostream>
#include <utility>
#include <vector>
#include <type_traits>
#include <cassert>
#include <string_view>
#include <format>

class Object {
public:
    int data;

    explicit Object(int d = 0) noexcept : data{d} {
        std::cout << std::format("Constructor({})\n", d);
    }

    Object(const Object& other) : data{other.data} {
        std::cout << "Copy constructor\n";
    }

    Object(Object&& other) noexcept : data{std::exchange(other.data, 0)} {
        std::cout << "Move constructor\n";
    }

    Object& operator=(const Object& other) {
        std::cout << "Copy assignment\n";
        data = other.data;
        return *this;
    }

    Object& operator=(Object&& other) noexcept {
        std::cout << "Move assignment\n";
        data = std::exchange(other.data, 0);
        return *this;
    }

    ~Object() { std::cout << "Destructor\n"; }

    void print(std::string_view label) const {
        std::cout << std::format("{} data={}\n", label, data);
    }
};

[[nodiscard]] Object create_object() {
    return Object{42};
}

[[nodiscard]] Object create_named_object() {
    Object temp{100};
    return temp;
}

[[nodiscard]] Object create_conditional(bool flag) {
    Object a{1};
    Object b{2};
    return flag ? std::move(a) : std::move(b);
}

[[nodiscard]] Object create_with_move() {
    Object temp{500};
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpessimizing-move"
#endif
    return std::move(temp);
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif
}

[[nodiscard]] Object create_const_object() {
    return Object{321};
}

[[nodiscard]] Object chain_create() {
    return create_object();
}

[[nodiscard]] Object nested_return() {
    return create_named_object();
}

[[nodiscard]] Object& get_static_object() {
    static Object obj{888};
    return obj;
}

void take_by_value(Object obj) {
    std::cout << std::format("take_by_value data={}\n", obj.data);
}

void take_by_reference(const Object& obj) {
    std::cout << std::format("take_by_reference data={}\n", obj.data);
}

template <typename T>
void forwarding_demo(T&& obj) {
    std::cout << "forwarding_demo: ";
    take_by_value(std::forward<T>(obj));
}

template <typename... Args>
[[nodiscard]] Object make_object(Args&&... args) {
    return Object{std::forward<Args>(args)...};
}

void vector_demo() {
    std::cout << "\n--- Vector Demo ---\n";
    std::vector<Object> v;
    v.reserve(3);

    std::cout << "push_back rvalue:\n";
    v.push_back(Object{10});

    std::cout << "push_back lvalue:\n";
    const Object temp{20};
    v.push_back(temp);

    std::cout << "emplace_back:\n";
    v.emplace_back(30);

    std::cout << std::format("size={}\n", v.size());
}

void moved_from_demo() {
    std::cout << "\n--- Moved-from State ---\n";
    Object a{111};
    const Object b{std::move(a)};
    std::cout << std::format("moved-from data={}\n", a.data);
    b.print("b");
}

void trait_demo() {
    std::cout << "\n--- Type Traits ---\n";
    std::cout << std::format("move_constructible={}\nnothrow_move_constructible={}\n",
                             std::is_move_constructible_v<Object>,
                             std::is_nothrow_move_constructible_v<Object>);
}

int main() {
    static_assert(std::is_move_constructible_v<Object>);
    static_assert(std::is_nothrow_move_constructible_v<Object>);

    std::cout << "\n--- Guaranteed RVO ---\n";
    Object obj1 = create_object();

    std::cout << "\n--- NRVO ---\n";
    Object obj2 = create_named_object();

    std::cout << "\n--- Conditional return (explicit move) ---\n";
    Object obj3 = create_conditional(true);

    std::cout << "\n--- Pass by value ---\n";
    take_by_value(obj1);

    std::cout << "\n--- Pass by reference ---\n";
    take_by_reference(obj1);

    std::cout << "\n--- Forced move (std::move inhibits RVO) ---\n";
    Object obj4 = create_with_move();

    std::cout << "\n--- Temporary ---\n";
    take_by_value(Object{999});

    std::cout << "\n--- Static object reference ---\n";
    Object& ref_obj = get_static_object();
    ref_obj.print("static");

    std::cout << "\n--- Chained creation ---\n";
    Object obj5 = chain_create();

    std::cout << "\n--- Const return (blocks move elision) ---\n";
    Object obj6 = create_const_object();

    std::cout << "\n--- Assignment ---\n";
    obj2 = obj1;
    obj2 = std::move(obj3);

    vector_demo();

    std::cout << "\n--- Perfect forwarding ---\n";
    forwarding_demo(obj1);
    forwarding_demo(Object{1234});

    std::cout << "\n--- Factory ---\n";
    Object obj7 = make_object(555);
    obj7.print("obj7");
    assert(obj7.data == 555);

    moved_from_demo();

    std::cout << "\n--- Nested return ---\n";
    Object obj8 = nested_return();
    obj8.print("obj8");

    trait_demo();

    std::cout << "\n--- End of main ---\n";
    return 0;
}
