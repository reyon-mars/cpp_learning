#include <iostream>
#include <utility>
#include <type_traits>

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

// ❌ Bad forwarding
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

template<typename T, typename... Args>
T create_object(Args&&... args) {
    return T(std::forward<Args>(args)...);
}

template<typename T>
void detect_value_category(T&& arg) {
    if constexpr (std::is_lvalue_reference<T>::value) {
        std::cout << "Lvalue detected\n";
    } else {
        std::cout << "Rvalue detected\n";
    }
}

void check(const Widget&) {
    std::cout << "check: const lvalue\n";
}

void check(Widget&) {
    std::cout << "check: non-const lvalue\n";
}

void check(Widget&&) {
    std::cout << "check: rvalue\n";
}

template<typename T>
void wrapper(T&& arg) {
    std::cout << "Inside wrapper -> ";
    process(std::forward<T>(arg));
}

Widget make_moved_widget(Widget w) {
    std::cout << "Inside make_moved_widget\n";
    return std::move(w);
}

// ----------- EXTRA ADVANCED ADDITIONS -----------

// Compare move vs forward
template<typename T>
void compare_move_forward(T&& arg) {
    std::cout << "Using std::move: ";
    process(std::move(arg));

    std::cout << "Using std::forward: ";
    process(std::forward<T>(arg));
}

// Forward multiple args
template<typename Func, typename... Args>
auto forward_multi(Func f, Args&&... args) {
    std::cout << "Forwarding multiple args\n";
    return f(std::forward<Args>(args)...);
}

// decltype(auto) perfect return
template<typename Func, typename... Args>
decltype(auto) forward_return(Func f, Args&&... args) {
    return f(std::forward<Args>(args)...);
}

// Reference collapsing demo
template<typename T>
void reference_demo(T&&) {
    std::cout << "Type is lvalue ref? "
              << std::is_lvalue_reference<T>::value << "\n";
}

// Double forward pitfall
template<typename T>
void double_forward(T&& arg) {
    std::cout << "First forward: ";
    process(std::forward<T>(arg));

    std::cout << "Second forward (⚠️ risky): ";
    process(std::forward<T>(arg)); // might use moved-from object
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
    detect_value_category(w);
    detect_value_category(Widget(400));

    std::cout << "\n--- Overload resolution ---\n";
    check(w);
    check(std::move(w));
    const Widget cw(500);
    check(cw);

    std::cout << "\n--- Wrapper forwarding ---\n";
    wrapper(w);
    wrapper(Widget(600));

    std::cout << "\n--- Move return demo ---\n";
    Widget w3 = make_moved_widget(Widget(700));

    // -------- EXTRA USAGE --------

    std::cout << "\n--- Move vs Forward ---\n";
    compare_move_forward(w2);
    compare_move_forward(Widget(800));

    std::cout << "\n--- Multi Forward ---\n";
    forward_multi(process, Widget(900));

    std::cout << "\n--- decltype(auto) return ---\n";
    forward_return(process, Widget(1000));

    std::cout << "\n--- Reference collapsing ---\n";
    reference_demo(w);
    reference_demo(Widget(1100));

    std::cout << "\n--- Double forward pitfall ---\n";
    double_forward(Widget(1200));

    // ----------------------------------

    return 0;
}
