// Noexcept Specifications Exercise
// Move semantics and exception specifications

#include <iostream>
#include <vector>

class Widget {
private:
    int data;
    
public:
    Widget(int d) : data(d) {}
    
    // Move constructor with noexcept
    Widget(Widget&& other) noexcept : data(other.data) {
        std::cout << "Move constructor (noexcept)\n";
    }
    
    // noexcept(condition)
    void process(int value) noexcept(std::is_integral_v<decltype(value)>) {
        data = value;
    }
};

// Function that may throw
void throwing_function() {
    throw std::runtime_error("Error!");
}

// Function with noexcept(false) - may throw
void maybe_throws() noexcept(false) {
    throwing_function();
}

int main() {
    std::vector<Widget> vec;
    Widget w(42);
    vec.push_back(std::move(w));  // Move constructor called
    
    return 0;
}
