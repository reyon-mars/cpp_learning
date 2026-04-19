#include <iostream>

// ----------- NEW ADDITIONS -----------

// Function with const parameter
void print_value(const int& val) {
    std::cout << "Value (const ref): " << val << "\n";
}

// Class demonstrating mutable and const function
class Demo {
public:
    int data;
    mutable int access_count = 0;

    Demo(int d) : data(d) {}

    int getData() const {
        access_count++; // allowed because mutable
        return data;
    }
};

// Function showing const_cast (for learning only)
void modify_const(const int* ptr) {
    int* modifiable = const_cast<int*>(ptr);
    *modifiable = 999; // ⚠️ dangerous if original was truly const
}

// ----------- EXTRA ADDITIONS -----------

// constexpr example
constexpr int square(int x) {
    return x * x;
}

// Volatile pointer demo
void volatile_demo() {
    volatile int val = 10;
    volatile int* ptr = &val;

    std::cout << "Volatile value via pointer: " << *ptr << "\n";
}

// Function overloading based on constness
void show(int& x) {
    std::cout << "Non-const reference: " << x << "\n";
}

void show(const int& x) {
    std::cout << "Const reference: " << x << "\n";
}

// Return const reference
const int& getConstRef(const int& x) {
    return x;
}

// ----------- MORE EXTRA ADDITIONS -----------

// Pointer const variations demo
void pointer_const_demo() {
    int a = 10, b = 20;

    int* p1 = &a;            // normal pointer
    const int* p2 = &a;      // pointer to const
    int* const p3 = &a;      // const pointer
    const int* const p4 = &a;// const pointer to const

    p1 = &b;     // allowed
    // *p2 = 30; // not allowed
    p2 = &b;     // allowed
    *p3 = 40;    // allowed
    // p3 = &b;  // not allowed

    std::cout << "Pointer const demo executed\n";
}

// constexpr with object
class ConstexprDemo {
public:
    int val;
    constexpr ConstexprDemo(int v) : val(v) {}
    constexpr int get() const { return val; }
};

// Safe const usage (no cast)
void safe_print(const int& x) {
    std::cout << "Safe const usage: " << x << "\n";
}

// ------------------------------------

int main() {
    int x = 10;
    
    const int a = 5;

    int* const ptr1 = &x;  
    *ptr1 = 15;

    const int* ptr2 = &x;  
    ptr2 = &a;

    const int* const ptr3 = &x;
    
    volatile int v = 100;
    v = 200;
    
    const volatile int cv = 300;
    
    std::cout << "x: " << x << "\n";
    std::cout << "a: " << a << "\n";
    std::cout << "v: " << v << "\n";
    std::cout << "cv: " << cv << "\n";

    // -------- NEW FEATURE USAGE --------

    print_value(x);

    Demo d(50);
    std::cout << "Demo data: " << d.getData() << "\n";
    std::cout << "Access count: " << d.access_count << "\n";

    int normal = 123;
    const int* p = &normal;
    modify_const(p);
    std::cout << "After const_cast modify: " << normal << "\n";

    // -------- EXTRA FEATURE USAGE --------

    std::cout << "\n--- constexpr demo ---\n";
    constexpr int val = square(5);
    std::cout << "Square (compile-time): " << val << "\n";

    std::cout << "\n--- volatile pointer demo ---\n";
    volatile_demo();

    std::cout << "\n--- const overload demo ---\n";
    show(x);   // non-const
    show(a);   // const

    std::cout << "\n--- const reference return ---\n";
    const int& ref = getConstRef(x);
    std::cout << "Const ref value: " << ref << "\n";

    // -------- MORE EXTRA USAGE --------

    std::cout << "\n--- pointer const variations ---\n";
    pointer_const_demo();

    std::cout << "\n--- constexpr object demo ---\n";
    constexpr ConstexprDemo obj(77);
    std::cout << "Constexpr object value: " << obj.get() << "\n";

    std::cout << "\n--- safe const usage ---\n";
    safe_print(a);

    // ----------------------------------

    return 0;
}
