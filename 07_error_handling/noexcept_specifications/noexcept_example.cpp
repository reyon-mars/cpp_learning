// Noexcept Specifications Exercise
// Move semantics and exception specifications

#include <iostream>
#include <vector>
#include <type_traits>
#include <stdexcept>

class Widget {
private:
    int data;

public:
    Widget(int d) : data(d) {}

    // Move constructor with noexcept
    Widget(Widget&& other) noexcept : data(other.data) {
        std::cout << "Move constructor (noexcept)\n";
        other.data = 0;
    }

    // Move assignment with noexcept
    Widget& operator=(Widget&& other) noexcept {
        std::cout << "Move assignment (noexcept)\n";
        data = other.data;
        other.data = 0;
        return *this;
    }

    // noexcept(condition)
    void process(int value) noexcept(std::is_integral_v<decltype(value)>) {
        data = value;
    }

    int get() const noexcept {
        return data;
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

// ---------------- SMALL ADDITIONS ----------------

// Safe wrapper that never throws
void safe_call() noexcept {
    try {
        maybe_throws();
    } catch (...) {
        std::cout << "Exception handled inside safe_call\n";
    }
}

// Helper to test noexcept at compile-time
template<typename T>
void check_noexcept() {
    std::cout << "Is T move construct noexcept? "
              << std::is_nothrow_move_constructible_v<T>
              << "\n";
}

// Simple factory function (may or may not throw)
Widget create_widget(int val) noexcept {
    return Widget(val);
}

// ---------------- MAIN ----------------
int main() {

    std::vector<Widget> vec;
    vec.reserve(2);

    Widget w1(42);
    Widget w2(100);

    vec.push_back(std::move(w1));  // Move constructor called
    vec.push_back(std::move(w2));

    std::cout << "Vector size: " << vec.size() << "\n";

    // Demonstrate noexcept operator
    std::cout << "Is Widget move constructor noexcept? "
              << noexcept(Widget(std::move(w1))) << "\n";

    // ---------------- ADDED USAGE ----------------

    // Check noexcept trait
    check_noexcept<Widget>();

    // Using noexcept function
    Widget w3 = create_widget(77);
    std::cout << "Created widget value: " << w3.get() << "\n";

    // Safe wrapper demo
    safe_call();

    // Test process()
    w3.process(999);
    std::cout << "Processed value: " << w3.get() << "\n";

    // Exception example
    try {
        maybe_throws();
    } catch (const std::exception& e) {
        std::cout << "Caught exception: " << e.what() << "\n";
    }

    return 0;
}
