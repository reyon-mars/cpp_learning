#include <iostream>
#include <utility>
#include <vector>     // ✅ ADDED
#include <type_traits> // ✅ ADDED
#include <cassert>     // ✅ ADDED

class Object {
public:
    int data;

    Object(int d = 0)
        : data(d) {

        std::cout
            << "Constructor("
            << d
            << ")\n";
    }

    Object(const Object& other)
        : data(other.data) {

        std::cout
            << "Copy constructor\n";
    }

    Object(Object&& other) noexcept
        : data(other.data) {

        std::cout
            << "Move constructor\n";

        other.data = 0; // ✅ ADDED
    }

    // ✅ ADDED: copy assignment
    Object& operator=(const Object& other) {

        std::cout
            << "Copy assignment\n";

        data = other.data;

        return *this;
    }

    // ✅ ADDED: move assignment
    Object& operator=(Object&& other) noexcept {

        std::cout
            << "Move assignment\n";

        data = other.data;

        other.data = 0; // ✅ ADDED

        return *this;
    }

    ~Object() {

        std::cout
            << "Destructor\n";
    }

    // ✅ ADDED helper
    void print(const std::string& label) const {

        std::cout
            << label
            << " data = "
            << data
            << "\n";
    }
};


// ✅ Guaranteed copy elision (C++17)
Object create_object() {

    return Object(42);
}


// ✅ NRVO candidate
Object create_named_object() {

    Object temp(100);

    return temp;
}


// ❌ Forces move (no elision possible)
Object create_conditional(bool flag) {

    Object a(1);
    Object b(2);

    if (flag)
        return a;
    else
        return b;
}


// ----------- NEW ADDITIONS -----------

void take_by_value(Object obj) {

    std::cout
        << "Inside take_by_value\n";
}


void take_by_reference(
    const Object& obj) {

    std::cout
        << "Inside take_by_reference\n";
}


// ❌ disables RVO using std::move
Object create_with_move() {

    Object temp(500);

    return std::move(temp);
}


Object create_temp_direct() {

    return Object(777);
}


Object& get_static_object() {

    static Object obj(888);

    return obj;
}


Object chain_create() {

    return create_object();
}


// ----------- EXTRA ADDITIONS -----------

// ❌ returning const disables move
const Object create_const_object() {

    return Object(321);
}


// Force copy vs move
void force_copy(Object obj) {

    std::cout
        << "Force copy/move into parameter\n";
}


// Vector behavior demo
void vector_demo() {

    std::cout
        << "\n--- Vector Demo ---\n";

    std::vector<Object> v;

    std::cout
        << "Push back temporary:\n";

    v.push_back(Object(10)); // move

    std::cout
        << "Push back lvalue:\n";

    Object temp(20);

    v.push_back(temp); // copy

    std::cout
        << "Emplace back:\n";

    v.emplace_back(30); // direct construct

    std::cout
        << "Vector size: "
        << v.size()
        << "\n";
}


// ----------- MORE EXTRA ADDITIONS -----------

// Perfect forwarding demo
template<typename T>
void forwarding_demo(T&& obj) {

    std::cout
        << "Forwarding demo\n";

    take_by_value(std::forward<T>(obj));
}


// Factory with forwarding
template<typename... Args>
Object make_object(Args&&... args) {

    return Object(
        std::forward<Args>(args)...);
}


// Move state demo
void moved_from_demo() {

    std::cout
        << "\n--- Moved-from State Demo ---\n";

    Object a(111);

    Object b(std::move(a));

    std::cout
        << "Moved-from object data: "
        << a.data
        << "\n";

    std::cout
        << "New object data: "
        << b.data
        << "\n";
}


// Return value optimization chain
Object nested_return() {

    return create_named_object();
}


// Type trait demo
void trait_demo() {

    std::cout
        << "\n--- Type Traits Demo ---\n";

    std::cout
        << "Is move constructible? "
        << std::is_move_constructible<Object>::value
        << "\n";

    std::cout
        << "Is noexcept move constructible? "
        << std::is_nothrow_move_constructible<Object>::value
        << "\n";
}


// --------------------------------------


// ======================================================
// MAIN
// ======================================================

int main() {

    std::cout
        << "\n--- Guaranteed RVO ---\n";

    Object obj1 = create_object();

    std::cout
        << "\n--- NRVO (usually optimized) ---\n";

    Object obj2 = create_named_object();

    std::cout
        << "\n--- Conditional Return (move expected) ---\n";

    Object obj3 = create_conditional(true);

    // -------- NEW FEATURE USAGE --------

    std::cout
        << "\n--- Pass by Value ---\n";

    take_by_value(obj1);

    std::cout
        << "\n--- Pass by Reference ---\n";

    take_by_reference(obj1);

    std::cout
        << "\n--- Forced Move (no RVO) ---\n";

    Object obj4 = create_with_move();

    std::cout
        << "\n--- Temporary Object ---\n";

    take_by_value(Object(999));

    std::cout
        << "\n--- Direct Temporary Return ---\n";

    Object obj5 = create_temp_direct();

    std::cout
        << "\n--- Static Object Reference ---\n";

    Object& refObj = get_static_object();

    std::cout
        << "Static object data: "
        << refObj.data
        << "\n";

    std::cout
        << "\n--- Chained Creation ---\n";

    Object obj6 = chain_create();

    // -------- EXTRA USAGE --------

    std::cout
        << "\n--- Const Return (may block move) ---\n";

    Object obj7 = create_const_object();

    std::cout
        << "\n--- Force Copy vs Move ---\n";

    force_copy(obj7);

    std::cout
        << "\n--- Assignment Tests ---\n";

    obj2 = obj1;               // copy assignment
    obj2 = std::move(obj3);   // move assignment

    vector_demo();

    // -------- MORE EXTRA USAGE --------

    std::cout
        << "\n--- Perfect Forwarding Demo ---\n";

    forwarding_demo(obj1);              // copy
    forwarding_demo(Object(1234));      // move

    std::cout
        << "\n--- Factory Function Demo ---\n";

    Object obj8 = make_object(555);

    obj8.print("obj8");

    moved_from_demo();

    std::cout
        << "\n--- Nested Return Demo ---\n";

    Object obj9 = nested_return();

    obj9.print("obj9");

    trait_demo();

    // ✅ compile-time checks
    static_assert(
        std::is_move_constructible<Object>::value,
        "Object should be movable"
    );

    assert(obj8.data == 555);

    // ----------------------------------

    std::cout
        << "\n--- End of main ---\n";

    return 0;
}
