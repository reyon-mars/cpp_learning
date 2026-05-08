#include <iostream>
#include <vector>   // ✅ ADDED
#include <string>   // ✅ ADDED

namespace my_lib {

class MyClass {
private:
    int value;

public:
    // constructor
    MyClass() : value(0) {
        std::cout << "MyClass default constructed\n";
    }

    // Constructor overloading
    MyClass(int v) : value(v) {
        std::cout << "MyClass constructed with value\n";
    }

    // ✅ ADDED: Copy constructor
    MyClass(const MyClass& other) : value(other.value) {
        std::cout << "MyClass copied\n";
    }

    // ✅ ADDED: Move constructor
    MyClass(MyClass&& other) noexcept : value(other.value) {
        other.value = 0;
        std::cout << "MyClass moved\n";
    }

    // ✅ ADDED: Destructor
    ~MyClass() {
        std::cout << "MyClass destroyed\n";
    }

    // Getter (needed for operator<<)
    int getValue() const {
        return value;
    }

    // ✅ ADDED: Setter
    void setValue(int v) {
        value = v;
    }

    // Comparison operator
    bool operator==(const MyClass& other) const {
        return value == other.value;
    }

    // ✅ ADDED: Inequality operator
    bool operator!=(const MyClass& other) const {
        return !(*this == other);
    }

    // ✅ ADDED: Addition operator
    MyClass operator+(const MyClass& other) const {
        return MyClass(value + other.value);
    }

    // ✅ ADDED: Prefix increment
    MyClass& operator++() {
        ++value;
        return *this;
    }

    // ✅ ADDED: Assignment operator
    MyClass& operator=(const MyClass& other) {
        if (this != &other) {
            value = other.value;
        }

        std::cout << "Assignment operator called\n";
        return *this;
    }

};

} // namespace my_lib



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

// ✅ ADDED: Pass by value demo
void showPassByValue(my_lib::MyClass obj) {
    std::cout << "Passed by value: " << obj << "\n";
}

// ✅ ADDED: Vector storage demo
void showCollection(const std::vector<my_lib::MyClass>& objects) {

    std::cout << "\nCollection contents:\n";

    for (const auto& obj : objects) {
        std::cout << obj << "\n";
    }
}

// ✅ ADDED: Factory helper
my_lib::MyClass createObject(int value) {
    return my_lib::MyClass(value);
}

// ---------------------------------------


// ================= MAIN =================

int main() {

    my_lib::MyClass obj;      // default
    my_lib::MyClass obj2(50); // existing object for comparison

    std::cout << obj << "\n";
    std::cout << obj2 << "\n";


    std::cout << "\nAdvanced Features:\n";

    my_lib::MyClass obj3(77);
    std::cout << obj3 << "\n";

    std::cout << "obj == obj2 ? "
              << (obj == obj2 ? "Yes\n" : "No\n");

    showCopy(obj3);

    // --------------------------------------------------

    // ✅ ADDED: Assignment operator demo
    obj = obj2;

    std::cout << "\nAfter assignment:\n";
    std::cout << obj << "\n";

    // --------------------------------------------------

    // ✅ ADDED: Addition operator demo
    my_lib::MyClass sum = obj2 + obj3;

    std::cout << "\nAddition result:\n";
    std::cout << sum << "\n";

    // --------------------------------------------------

    // ✅ ADDED: Increment operator demo
    ++obj3;

    std::cout << "\nAfter increment:\n";
    std::cout << obj3 << "\n";

    // --------------------------------------------------

    // ✅ ADDED: Inequality demo
    std::cout << "obj2 != obj3 ? "
              << (obj2 != obj3 ? "Yes\n" : "No\n");

    // --------------------------------------------------

    // ✅ ADDED: Setter demo
    obj.setValue(99);

    std::cout << "\nUpdated obj:\n";
    std::cout << obj << "\n";

    // --------------------------------------------------

    // ✅ ADDED: Pass by value
    showPassByValue(obj2);

    // --------------------------------------------------

    // ✅ ADDED: Collection demo
    std::vector<my_lib::MyClass> objects;

    objects.push_back(obj);
    objects.push_back(obj2);
    objects.push_back(obj3);

    showCollection(objects);

    // --------------------------------------------------

    // ✅ ADDED: Factory function demo
    my_lib::MyClass generated = createObject(500);

    std::cout << "\nGenerated object:\n";
    std::cout << generated << "\n";

    // --------------------------------------------------

    // ✅ ADDED: Move semantics demo
    my_lib::MyClass moved = std::move(generated);

    std::cout << "\nMoved object:\n";
    std::cout << moved << "\n";

    // --------------------------------------------------

    return 0;
}
