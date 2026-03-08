// Exception Safety Exercise
// No-throw guarantee, strong guarantee, basic guarantee

#include <iostream>
#include <vector>
#include <stdexcept>

class SafeVector {
private:
    std::vector<int> data;

public:
    // Strong guarantee: either succeeds or has no effect
    void add_element_strong(int value) {
        std::vector<int> temp = data;  // Copy
        try {
            temp.push_back(value);
            data = std::move(temp);  // Commit
        } catch (...) {
            // Original data unchanged
            throw;
        }
    }

    // Basic guarantee: object remains valid but state may change
    void add_element_basic(int value) {
        try {
            data.push_back(value);
        } catch (...) {
            std::cout << "Exception occurred during push_back\n";
        }
    }

    // No-throw guarantee
    void clear() noexcept {
        data.clear();
    }

    // Reserve capacity (helps prevent reallocation exceptions)
    void reserve(std::size_t n) {
        data.reserve(n);
    }

    std::size_t size() const noexcept {
        return data.size();
    }

    void print() const {
        for (int v : data) {
            std::cout << v << " ";
        }
        std::cout << "\n";
    }
};

int main() {
    SafeVector sv;

    std::cout << "Adding elements with strong guarantee:\n";
    sv.add_element_strong(1);
    sv.add_element_strong(2);
    sv.print();

    std::cout << "Adding elements with basic guarantee:\n";
    sv.add_element_basic(3);
    sv.add_element_basic(4);
    sv.print();

    std::cout << "Vector size: " << sv.size() << "\n";

    std::cout << "Clearing vector (noexcept):\n";
    sv.clear();
    sv.print();

    return 0;
}
