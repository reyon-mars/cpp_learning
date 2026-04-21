#include <iostream>
#include <utility>
#include <vector>   // ✅ ADDED

class Object {
public:
    int data;

    Object(int d = 0) : data(d) {
        std::cout << "Constructor(" << d << ")\n";
    }

    Object(const Object& other) : data(other.data) {
        std::cout << "Copy constructor\n";
    }

    Object(Object&& other) noexcept : data(other.data) {
        std::cout << "Move constructor\n";
    }

    // ✅ ADDED: copy assignment
    Object& operator=(const Object& other) {
        std::cout << "Copy assignment\n";
        data = other.data;
        return *this;
    }

    // ✅ ADDED: move assignment
    Object& operator=(Object&& other) noexcept {
        std::cout << "Move assignment\n";
        data = other.data;
        return *this;
    }

    ~Object() {
        std::cout << "Destructor\n";
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
    std::cout << "Inside take_by_value\n";
}

void take_by_reference(const Object& obj) {
    std::cout << "Inside take_by_reference\n";
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
    std::cout << "Force copy/move into parameter\n";
}

// Vector behavior demo
void vector_demo() {
    std::cout << "\n--- Vector Demo ---\n";

    std::vector<Object> v;

    std::cout << "Push back temporary:\n";
    v.push_back(Object(10)); // move

    std::cout << "Push back lvalue:\n";
    Object temp(20);
    v.push_back(temp); // copy

    std::cout << "Emplace back:\n";
    v.emplace_back(30); // direct construct
}

// --------------------------------------

int main() {

    std::cout << "\n--- Guaranteed RVO ---\n";
    Object obj1 = create_object();

    std::cout << "\n--- NRVO (usually optimized) ---\n";
    Object obj2 = create_named_object();

    std::cout << "\n--- Conditional Return (move expected) ---\n";
    Object obj3 = create_conditional(true);

    // -------- NEW FEATURE USAGE --------

    std::cout << "\n--- Pass by Value ---\n";
    take_by_value(obj1);

    std::cout << "\n--- Pass by Reference ---\n";
    take_by_reference(obj1);

    std::cout << "\n--- Forced Move (no RVO) ---\n";
    Object obj4 = create_with_move();

    std::cout << "\n--- Temporary Object ---\n";
    take_by_value(Object(999));

    std::cout << "\n--- Direct Temporary Return ---\n";
    Object obj5 = create_temp_direct();

    std::cout << "\n--- Static Object Reference ---\n";
    Object& refObj = get_static_object();
    std::cout << "Static object data: " << refObj.data << "\n";

    std::cout << "\n--- Chained Creation ---\n";
    Object obj6 = chain_create();

    // -------- EXTRA USAGE --------

    std::cout << "\n--- Const Return (may block move) ---\n";
    Object obj7 = create_const_object();

    std::cout << "\n--- Force Copy vs Move ---\n";
    force_copy(obj7);

    std::cout << "\n--- Assignment Tests ---\n";
    obj2 = obj1;               // copy assignment
    obj2 = std::move(obj3);   // move assignment

    vector_demo();

    // ----------------------------------

    std::cout << "\n--- End of main ---\n";
    return 0;
}
