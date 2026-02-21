// Undefined Behavior Sandbox
// Exploring and understanding undefined behavior

#include <iostream>

// WARNING: These examples demonstrate undefined behavior
// They should NOT be used in production code

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
    
    std::cout << "Undefined behavior examples (commented out for safety)\n";
    
    return 0;
}
