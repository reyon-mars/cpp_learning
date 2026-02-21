// Rvalue References and Value Categories Exercise
// lvalue, rvalue, xvalue semantics

#include <iostream>
#include <utility>

class Widget {
public:
    int data;
    
    Widget(int d) : data(d) { std::cout << "Constructor\n"; }
    Widget(const Widget& other) : data(other.data) { std::cout << "Copy constructor\n"; }
    Widget(Widget&& other) noexcept : data(other.data) { std::cout << "Move constructor\n"; }
};

int main() {
    Widget w1(10);              // lvalue
    Widget w2 = w1;             // Copy
    Widget w3 = std::move(w1);  // Move
    
    return 0;
}
