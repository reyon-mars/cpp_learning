// Move Semantics and Perfect Forwarding Exercise
// Efficient parameter passing with move and forward

#include <iostream>
#include <utility>

class Widget {
public:
    int data;

    Widget(int d) : data(d) {
        std::cout << "Constructor\n";
    }

    Widget(const Widget&) {
        std::cout << "Copy constructor\n";
    }

    Widget(Widget&&) noexcept {
        std::cout << "Move constructor\n";
    }
};

// ❌ Bad forwarding (loses rvalue-ness)
template<typename Func, typename... Args>
auto bad_forward_call(Func func, Args&&... args) {
    return func(args...);  // args are always lvalues here
}

// ✅ Perfect forwarding
template<typename Func, typename... Args>
auto forward_call(Func func, Args&&... args) {
    return func(std::forward<Args>(args)...);
}

void process(const Widget&) {
    std::cout << "process(const Widget&)\n";
}

void process(Widget&&) {
    std::cout << "process(Widget&&)\n";
}

int main() {

    std::cout << "\n--- Creating lvalue ---\n";
    Widget w(42);

    std::cout << "\n--- Forward lvalue ---\n";
    forward_call(process, w);  // should call const& version

    std::cout << "\n--- Forward rvalue ---\n";
    forward_call(process, Widget(100));  // should call && version

    std::cout << "\n--- Bad forward rvalue ---\n";
    bad_forward_call(process, Widget(200));  // loses rvalue

    std::cout << "\n--- Explicit std::move ---\n";
    forward_call(process, std::move(w));  // forces rvalue

    return 0;
}
