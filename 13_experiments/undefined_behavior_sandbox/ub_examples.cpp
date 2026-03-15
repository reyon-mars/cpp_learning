// Undefined Behavior Sandbox
// Exploring and understanding undefined behavior

#include <iostream>
#include <climits>

int main() {

    // Example 1: Buffer overflow
    // char buffer[5];
    // strcpy(buffer, "Hello World");  // UB: exceeds buffer size

    // Example 2: Null pointer dereference
    // int* ptr = nullptr;
    // *ptr = 10;  // UB

    // Example 3: Use after free
    // int* p = new int(10);
    // delete p;
    // *p = 20;  // UB

    // Example 4: Signed integer overflow
    // int max_int = INT_MAX;
    // int overflow = max_int + 1;  // UB in C++

    std::cout << "Undefined behavior examples (commented out for safety)\n\n";

    // ---- Safe experiments ----

    // Demonstrating integer limits
    std::cout << "INT_MAX: " << INT_MAX << "\n";
    std::cout << "INT_MIN: " << INT_MIN << "\n";

    // Safe overflow check
    int a = INT_MAX;

    if (a > INT_MAX - 1) {
        std::cout << "Adding 1 would cause signed integer overflow (UB)\n";
    }

    // Dangling pointer example (without dereferencing)
    int* ptr = new int(42);
    delete ptr;

    std::cout << "Pointer after delete (dangling): " << ptr << "\n";
    std::cout << "Accessing it would be undefined behavior\n";

    return 0;
}
