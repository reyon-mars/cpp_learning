// Move Semantics (C++11) Exercise
// Efficient resource transfer

#include <iostream>
#include <utility>

class Vector {
private:
    int* data;
    size_t size;
    
public:
    Vector(size_t s) : size(s), data(new int[s]) {
        std::cout << "Constructor\n";
    }
    
    Vector(const Vector&) = delete;  // Disable copy
    
    Vector(Vector&& other) noexcept : size(other.size), data(other.data) {
        other.data = nullptr;
        other.size = 0;
        std::cout << "Move constructor\n";
    }
    
    ~Vector() { delete[] data; }
};

int main() {
    Vector v1(100);
    Vector v2 = std::move(v1);  // Move semantics
    
    return 0;
}
