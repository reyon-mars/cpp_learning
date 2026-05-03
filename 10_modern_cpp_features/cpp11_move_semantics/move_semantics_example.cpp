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

    // ===== VERY SMALL NEW ADDITIONS =====

    // access element safely
    int& operator[](size_t index) {
        return data[index];
    }

    // const version
    const int& operator[](size_t index) const {
        return data[index];
    }

    // fill with values
    void fill(int value) {
        for (size_t i = 0; i < size; ++i)
            data[i] = value;
    }

    // ===================================

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

    // ===== VERY SMALL NEW ADDITIONS =====

    // fill and access demo
    v3.fill(5);
    std::cout << "First element of v3: " << v3[0] << "\n";

    // check sizes after moves
    std::cout << "v2 size after move: " << v2.get_size() << "\n";

    // ---- EXTRA SMALL ADDITIONS ----

    // check if v3 is empty
    std::cout << "v3 is empty? "
              << (v3.is_empty() ? "Yes" : "No") << "\n";

    // modify element
    v3[0] = 99;
    std::cout << "Modified first element of v3: " << v3[0] << "\n";

    // print state again
    v3.print_state();

    // create another vector and move into it
    Vector v4(10);
    v4 = std::move(v3);

    std::cout << "After moving v3 to v4:\n";
    v4.print_state();
    std::cout << "v3 size: " << v3.get_size() << "\n";

    // -----------------------------------

    return 0;
}
