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

    // 🔹 NEW: safe access with exception
    int at(std::size_t index) const {
        if (index >= data.size()) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    // 🔹 NEW: strong guarantee insert at front
    void push_front_strong(int value) {
        std::vector<int> temp = data;
        try {
            temp.insert(temp.begin(), value);
            data = std::move(temp);
        } catch (...) {
            throw;
        }
    }

    // 🔹 NEW: no-throw swap
    void swap(SafeVector& other) noexcept {
        data.swap(other.data);
    }

    // ---------------- EXTRA ADDITIONS ----------------

    // 🔹 NEW: pop_back with basic guarantee
    void pop_back_basic() {
        if (!data.empty()) {
            data.pop_back();
        }
    }

    // 🔹 NEW: strong guarantee replace element
    void replace_strong(std::size_t index, int value) {
        if (index >= data.size()) {
            throw std::out_of_range("Index out of range");
        }

        std::vector<int> temp = data;
        temp[index] = value;
        data = std::move(temp);
    }

    // 🔹 NEW: safe getter (no exception, returns optional-like behavior)
    bool try_get(std::size_t index, int& out) const noexcept {
        if (index >= data.size()) return false;
        out = data[index];
        return true;
    }

    // 🔹 NEW: check empty (noexcept)
    bool empty() const noexcept {
        return data.empty();
    }

    // ------------------------------------------------
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

    // 🔹 NEW: reserve demo
    sv.reserve(10);
    std::cout << "Reserved capacity for 10 elements\n";

    // 🔹 NEW: push front strong guarantee
    sv.push_front_strong(0);
    sv.print();

    // 🔹 NEW: safe access demo
    try {
        std::cout << "Element at index 2: " << sv.at(2) << "\n";
        std::cout << "Access invalid index:\n";
        std::cout << sv.at(100) << "\n";  // will throw
    } catch (const std::exception& e) {
        std::cout << "Caught exception: " << e.what() << "\n";
    }

    std::cout << "Clearing vector (noexcept):\n";
    sv.clear();
    sv.print();

    // 🔹 NEW: swap demo
    SafeVector other;
    other.add_element_basic(99);
    sv.swap(other);

    std::cout << "After swap, sv contains:\n";
    sv.print();

    // ---------------- EXTRA USAGE ----------------

    std::cout << "\n--- Extra Tests ---\n";

    sv.add_element_basic(10);
    sv.add_element_basic(20);
    sv.add_element_basic(30);
    sv.print();

    // pop_back
    sv.pop_back_basic();
    std::cout << "After pop_back:\n";
    sv.print();

    // strong replace
    sv.replace_strong(0, 999);
    std::cout << "After replace:\n";
    sv.print();

    // try_get
    int value;
    if (sv.try_get(1, value)) {
        std::cout << "try_get success: " << value << "\n";
    } else {
        std::cout << "try_get failed\n";
    }

    // empty check
    std::cout << "Is empty? "
              << (sv.empty() ? "Yes" : "No") << "\n";

    // --------------------------------------------

    return 0;
}
