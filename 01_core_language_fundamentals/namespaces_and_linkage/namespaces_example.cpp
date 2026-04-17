#include <iostream>

namespace my_lib {

class MyClass {
private:
    int value;

public:
    // Original constructor
    MyClass() : value(0) {
        std::cout << "MyClass default constructed\n";
    }

    // ----------- MORE ADDITIONS -----------

    // Constructor overloading
    MyClass(int v) : value(v) {
        std::cout << "MyClass constructed with value\n";
    }

    // Getter (needed for operator<<)
    int getValue() const {
        return value;
    }

    // Comparison operator
    bool operator==(const MyClass& other) const {
        return value == other.value;
    }

    // ------------------------------------
};

} // namespace my_lib


// ----------- GLOBAL ADDITIONS -----------

// Operator overloading for printing
std::ostream& operator<<(std::ostream& os, const my_lib::MyClass& obj) {
    os << "MyClass(value=" << obj.getValue() << ")";
    return os;
}

// Function to demonstrate copying
void showCopy(const my_lib::MyClass& obj) {
    my_lib::MyClass copy = obj; // copy constructor
    std::cout << "Copied object: " << copy << "\n";
}

// ---------------------------------------


// ================= MAIN =================

int main() {

    my_lib::MyClass obj;      // default
    my_lib::MyClass obj2(50); // existing object for comparison

    std::cout << obj << "\n";
    std::cout << obj2 << "\n";

    // ================= ADDITIONS =================

    std::cout << "\nAdvanced Features:\n";

    // ✅ ADDED: constructor overloading
    my_lib::MyClass obj3(77);
    std::cout << obj3 << "\n";

    // ✅ ADDED: comparison
    std::cout << "obj == obj2 ? "
              << (obj == obj2 ? "Yes\n" : "No\n");

    // ✅ ADDED: copy demonstration
    showCopy(obj3);

    // =================================================

    return 0;
}
