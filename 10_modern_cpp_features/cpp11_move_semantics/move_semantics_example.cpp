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

    // ---- small added helper ----
    Vector& operator=(Vector&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            size = other.size;

            other.data = nullptr;
            other.size = 0;
            std::cout << "Move assignment\n";
        }
        return *this;
    }
    // ----------------------------

    // small utility functions
    size_t get_size() const { return size; }

    bool is_empty() const { return data == nullptr; }

    void print_state() const {
        std::cout << "Vector size: " << size
                  << ", data pointer: " << data << "\n";
    }

    ~Vector() { delete[] data; }
};

int main() {
    Vector v1(100);
    Vector v2 = std::move(v1);  // Move semantics

    // ---- small added usage ----
    v2.print_state();
    std::cout << "v1 moved-from state: "
              << (v1.is_empty() ? "empty" : "not empty") << "\n";

    Vector v3(50);
    v3 = std::move(v2); // move assignment

    v3.print_state();
    // ----------------------------

    return 0;
}
