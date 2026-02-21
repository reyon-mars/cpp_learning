// Move Semantics and Perfect Forwarding Exercise
// Efficient parameter passing with move and forward

#include <iostream>
#include <utility>

class Widget {
public:
    int data;
    
    Widget(int d) : data(d) { std::cout << "Constructor\n"; }
    Widget(const Widget&) { std::cout << "Copy constructor\n"; }
    Widget(Widget&&) noexcept { std::cout << "Move constructor\n"; }
};

// Perfect forwarding
template<typename Func, typename... Args>
auto forward_call(Func func, Args&&... args) {
    return func(std::forward<Args>(args)...);
}

void process(const Widget&) { std::cout << "process(const Widget&)\n"; }
void process(Widget&&) { std::cout << "process(Widget&&)\n"; }

int main() {
    Widget w(42);
    
    forward_call(process, w);           // Forwards as lvalue
    forward_call(process, Widget(100)); // Forwards as rvalue
    
    return 0;
}
