// Copy Elision and RVO Exercise
// Return value optimization and copy elision

#include <iostream>

class Object {
public:
    int data;
    
    Object(int d = 0) : data(d) { std::cout << "Constructor(" << d << ")\n"; }
    Object(const Object& other) : data(other.data) { std::cout << "Copy constructor\n"; }
    Object(Object&& other) noexcept : data(other.data) { std::cout << "Move constructor\n"; }
    ~Object() { std::cout << "Destructor\n"; }
};

Object create_object() {
    return Object(42);  // RVO (Return Value Optimization)
}

int main() {
    std::cout << "Creating object...\n";
    Object obj = create_object();  // Likely no copy/move (RVO/elision)
    std::cout << "Object created\n";
    
    return 0;
}
