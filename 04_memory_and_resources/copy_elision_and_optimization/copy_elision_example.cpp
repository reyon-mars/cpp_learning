// Copy Elision and RVO Exercise
// Return value optimization and copy elision

#include <iostream>
#include <utility>

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

// Pass by value (copy/move happens)
void take_by_value(Object obj) {
    std::cout << "Inside take_by_value\n";
}

// Pass by reference (no copy)
void take_by_reference(const Object& obj) {
    std::cout << "Inside take_by_reference\n";
}

// ❌ disables RVO using std::move
Object create_with_move() {
    Object temp(500);
    return std::move(temp);  // forces move
}

// ------------------------------------

int main() {

    std::cout << "\n--- Guaranteed RVO ---\n";
    Object obj1 = create_object();

    std::cout << "\n--- NRVO (usually optimized) ---\n";
    Object obj2 = create_named_object();

    std::cout << "\n--- Conditional Return (move expected) ---\n";
    Object obj3 = create_conditional(true);

    // -------- NEW FEATURE USAGE --------

    std::cout << "\n--- Pass by Value ---\n";
    take_by_value(obj1);   // copy

    std::cout << "\n--- Pass by Reference ---\n";
    take_by_reference(obj1); // no copy

    std::cout << "\n--- Forced Move (no RVO) ---\n";
    Object obj4 = create_with_move();

    std::cout << "\n--- Temporary Object ---\n";
    take_by_value(Object(999)); // may use move

    // ----------------------------------

    std::cout << "\n--- End of main ---\n";
    return 0;
}
