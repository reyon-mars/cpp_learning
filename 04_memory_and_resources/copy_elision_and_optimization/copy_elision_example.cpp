// Copy Elision and RVO Exercise
// Return value optimization and copy elision

#include <iostream>

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
    return Object(42);  // temporary returned directly
}

// ✅ NRVO candidate
Object create_named_object() {
    Object temp(100);
    return temp;  // Named Return Value Optimization (not guaranteed)
}

// ❌ Forces move (no elision possible)
Object create_conditional(bool flag) {
    Object a(1);
    Object b(2);

    if (flag)
        return a;   // may move
    else
        return b;   // may move
}

int main() {

    std::cout << "\n--- Guaranteed RVO ---\n";
    Object obj1 = create_object();

    std::cout << "\n--- NRVO (usually optimized) ---\n";
    Object obj2 = create_named_object();

    std::cout << "\n--- Conditional Return (move expected) ---\n";
    Object obj3 = create_conditional(true);

    std::cout << "\n--- End of main ---\n";
    return 0;
}
