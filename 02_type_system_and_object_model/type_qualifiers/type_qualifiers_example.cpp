// Type Qualifiers Exercise
// const, volatile, and const-volatile combinations

#include <iostream>

int main() {
    int x = 10;
    
    // const objects
    const int a = 5;
    // a = 10;  // Error
    
    // const pointers
    int* const ptr1 = &x;  // Can't change pointer
    // ptr1 = &a;  // Error
    
    // Pointer to const
    const int* ptr2 = &x;  // Can't change value through pointer
    // *ptr2 = 20;  // Error
    
    // Pointer to const, const pointer
    const int* const ptr3 = &x;
    
    // volatile
    volatile int v = 100;
    
    return 0;
}
