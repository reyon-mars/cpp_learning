// Type Qualifiers Exercise
// const, volatile, and const-volatile combinations

#include <iostream>

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
    
    return 0;
}
