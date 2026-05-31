#include <iostream>
#include <vector>
#include <string>
#include <algorithm>   // ✅ NEW

namespace my_lib {

class MyClass {
private:
    int value;

    // ✅ NEW: track object count
    static int objectCount;

public:
    // constructor
    MyClass() : value(0) {
        ++objectCount;
        std::cout << "MyClass default constructed\n";
    }

    // Constructor overloading
    MyClass(int v) : value(v) {
        ++objectCount;
        std::cout << "MyClass constructed with value\n";
    }

    // Copy constructor
    MyClass(const MyClass& other) : value(other.value) {
        ++objectCount;
        std::cout << "MyClass copied\n";
    }

    // Move constructor
    MyClass(MyClass&& other) noexcept : value(other.value) {
        ++objectCount;
        other.value = 0;
        std::cout << "MyClass moved\n";
    }

    // Destructor
    ~MyClass() {
        --objectCount;
        std::cout << "MyClass destroyed\n";
    }

    int getValue() const {
        return value;
    }

    void setValue(int v) {
        value = v;
    }

    bool operator==(const MyClass& other) const {
        return value == other.value;
    }

    bool operator!=(const MyClass& other) const {
        return !(*this == other);
    }

    MyClass operator+(const MyClass& other) const {
        return MyClass(value + other.value);
    }

    MyClass& operator++() {
        ++value;
        return *this;
    }

    MyClass& operator=(const MyClass& other) {
        if (this != &other) {
            value = other.value;
        }

        std::cout << "Assignment operator called\n";
        return *this;
    }

    // --------------------------------------------------
    // ✅ NEW ADDITIONS

    bool operator<(const MyClass& other) const {
        return value < other.value;
    }

    MyClass& operator+=(const MyClass& other) {
        value += other.value;
        return *this;
    }

    void reset() {
        value = 0;
    }

    static int getObjectCount() {
        return objectCount;
    }

    void displayInfo() const {
        std::cout << "Current value = "
                  << value << "\n";
    }
};

// Static member definition
int MyClass::objectCount = 0;

} // namespace my_lib


// Operator overloading for printing
std::ostream& operator<<(std::ostream& os,
                         const my_lib::MyClass& obj) {
    os << "MyClass(value=" << obj.getValue() << ")";
    return os;
}

// Function to demonstrate copying
void showCopy(const my_lib::MyClass& obj) {
    my_lib::MyClass copy = obj;
    std::cout << "Copied object: " << copy << "\n";
}

// Pass by value demo
void showPassByValue(my_lib::MyClass obj) {
    std::cout << "Passed by value: " << obj << "\n";
}

// Vector storage demo
void showCollection(
    const std::vector<my_lib::MyClass>& objects) {

    std::cout << "\nCollection contents:\n";

    for (const auto& obj : objects) {
        std::cout << obj << "\n";
    }
}

// Factory helper
my_lib::MyClass createObject(int value) {
    return my_lib::MyClass(value);
}

// --------------------------------------------------
// ✅ NEW HELPER

void printDivider() {
    std::cout
        << "-----------------------------\n";
}

// ================= MAIN =================

int main() {

    my_lib::MyClass obj;
    my_lib::MyClass obj2(50);

    std::cout << obj << "\n";
    std::cout << obj2 << "\n";

    std::cout << "\nAdvanced Features:\n";

    my_lib::MyClass obj3(77);

    std::cout << obj3 << "\n";

    std::cout << "obj == obj2 ? "
              << (obj == obj2 ? "Yes\n" : "No\n");

    showCopy(obj3);

    // Assignment operator demo
    obj = obj2;

    std::cout << "\nAfter assignment:\n";
    std::cout << obj << "\n";

    // Addition operator demo
    my_lib::MyClass sum = obj2 + obj3;

    std::cout << "\nAddition result:\n";
    std::cout << sum << "\n";

    // Increment operator demo
    ++obj3;

    std::cout << "\nAfter increment:\n";
    std::cout << obj3 << "\n";

    // Inequality demo
    std::cout << "obj2 != obj3 ? "
              << (obj2 != obj3 ? "Yes\n" : "No\n");

    // Setter demo
    obj.setValue(99);

    std::cout << "\nUpdated obj:\n";
    std::cout << obj << "\n";

    // Pass by value
    showPassByValue(obj2);

    // Collection demo
    std::vector<my_lib::MyClass> objects;

    objects.push_back(obj);
    objects.push_back(obj2);
    objects.push_back(obj3);

    showCollection(objects);

    // Factory function demo
    my_lib::MyClass generated =
        createObject(500);

    std::cout << "\nGenerated object:\n";
    std::cout << generated << "\n";

    // Move semantics demo
    my_lib::MyClass moved =
        std::move(generated);

    std::cout << "\nMoved object:\n";
    std::cout << moved << "\n";

    // --------------------------------------------------
    // ✅ NEW FEATURES

    printDivider();

    std::cout << "Using += operator\n";

    obj2 += obj3;

    std::cout << obj2 << "\n";

    printDivider();

    std::cout << "Comparison using < operator:\n";

    std::cout << (obj < obj3
                  ? "obj is smaller\n"
                  : "obj is not smaller\n");

    printDivider();

    std::cout << "Resetting obj...\n";

    obj.reset();

    std::cout << obj << "\n";

    printDivider();

    std::cout << "Displaying object info:\n";

    obj3.displayInfo();

    printDivider();

    std::cout << "Sorting collection...\n";

    std::sort(objects.begin(),
              objects.end());

    showCollection(objects);

    printDivider();

    std::cout << "Current object count: "
              << my_lib::MyClass::getObjectCount()
              << "\n";

    // --------------------------------------------------

    return 0;
}
