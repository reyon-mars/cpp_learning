// Exception Safety Exercise
// No-throw guarantee, strong guarantee, basic guarantee

#include <iostream>
#include <vector>

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
    
    // No-throw guarantee
    void clear() noexcept {
        data.clear();
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
    sv.add_element_strong(1);
    sv.add_element_strong(2);
    sv.print();
    
    return 0;
}
