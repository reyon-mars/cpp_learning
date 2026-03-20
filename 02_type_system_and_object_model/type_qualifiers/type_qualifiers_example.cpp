// Type Qualifiers Exercise
// const, volatile, and const-volatile combinations

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

// ------------------------------------

int main() {
    int x = 10;
    
    // const objects
    const int a = 5;
    // a = 10;  // Error
    
    // const pointer (pointer itself is const)
    int* const ptr1 = &x;  
    *ptr1 = 15;            // ✅ allowed (value can change)
    // ptr1 = &a;          // ❌ Error
    
    // pointer to const (value is const)
    const int* ptr2 = &x;  
    // *ptr2 = 20;         // ❌ Error
    ptr2 = &a;             // ✅ allowed (pointer can change)
    
    // const pointer to const
    const int* const ptr3 = &x;
    
    // volatile variable
    volatile int v = 100;
    v = 200;               // allowed, but compiler won’t optimize accesses
    
    // const-volatile
    const volatile int cv = 300;
    // cv = 400;            // ❌ Error (const)
    
    // reading values
    std::cout << "x: " << x << "\n";
    std::cout << "a: " << a << "\n";
    std::cout << "v: " << v << "\n";
    std::cout << "cv: " << cv << "\n";


    // -------- NEW FEATURE USAGE --------

    // const reference
    print_value(x);

    // const class usage
    Demo d(50);
    std::cout << "Demo data: " << d.getData() << "\n";
    std::cout << "Access count: " << d.access_count << "\n";

    // const_cast demonstration
    int normal = 123;
    const int* p = &normal;
    modify_const(p);
    std::cout << "After const_cast modify: " << normal << "\n";

    // ----------------------------------

    return 0;
}
