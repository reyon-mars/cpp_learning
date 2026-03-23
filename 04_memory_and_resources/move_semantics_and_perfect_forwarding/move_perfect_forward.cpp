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
    return func(args...);
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

// ----------- NEW ADDITIONS -----------

// Emplace-style creator (like vector::emplace_back)
template<typename T, typename... Args>
T create_object(Args&&... args) {
    return T(std::forward<Args>(args)...);
}

// Universal reference demo
template<typename T>
void detect_value_category(T&& arg) {
    if constexpr (std::is_lvalue_reference<T>::value) {
        std::cout << "Lvalue detected\n";
    } else {
        std::cout << "Rvalue detected\n";
    }
}

// Overload resolution clarity
void check(const Widget&) {
    std::cout << "check: const lvalue\n";
}

void check(Widget&) {
    std::cout << "check: non-const lvalue\n";
}

void check(Widget&&) {
    std::cout << "check: rvalue\n";
}

// ------------------------------------

int main() {

    std::cout << "\n--- Creating lvalue ---\n";
    Widget w(42);

    std::cout << "\n--- Forward lvalue ---\n";
    forward_call(process, w);

    std::cout << "\n--- Forward rvalue ---\n";
    forward_call(process, Widget(100));

    std::cout << "\n--- Bad forward rvalue ---\n";
    bad_forward_call(process, Widget(200));

    std::cout << "\n--- Explicit std::move ---\n";
    forward_call(process, std::move(w));

    // -------- NEW FEATURE USAGE --------

    std::cout << "\n--- Emplace-style creation ---\n";
    Widget w2 = create_object<Widget>(300);

    std::cout << "\n--- Value category detection ---\n";
    detect_value_category(w);              // lvalue
    detect_value_category(Widget(400));    // rvalue

    std::cout << "\n--- Overload resolution ---\n";
    check(w);                 // lvalue
    check(std::move(w));      // rvalue
    const Widget cw(500);
    check(cw);                // const lvalue

    // ----------------------------------

    return 0;
}
