#include <iostream>
#include <utility>
#include <string>

class Widget {
public:
    int data;
    
    Widget(int d) : data(d) {
        std::cout << "Constructor\n";
    }

    Widget(const Widget& other) : data(other.data) {
        std::cout << "Copy constructor\n";
    }

    Widget(Widget&& other) noexcept : data(other.data) {
        std::cout << "Move constructor\n";
        other.data = 0;
    }

    Widget& operator=(const Widget& other) {
        std::cout << "Copy assignment\n";
        data = other.data;
        return *this;
    }

    Widget& operator=(Widget&& other) noexcept {
        std::cout << "Move assignment\n";
        data = other.data;
        other.data = 0;
        return *this;
    }

    // ----------- EXTRA ADDED -----------
    ~Widget() {
        std::cout << "Destructor (data=" << data << ")\n";
    }
};

// ------------------------------------

void consume(Widget&& w) {
    std::cout << "Consuming Widget with data: " << w.data << "\n";
}

// ----------- NEW ADDITIONS -----------

// Overload to detect lvalue
void detect(Widget& w) {
    std::cout << "Lvalue detected with data: " << w.data << "\n";
}

// Overload to detect rvalue
void detect(Widget&& w) {
    std::cout << "Rvalue detected with data: " << w.data << "\n";
}

// Function returning rvalue
Widget createWidget(int val) {
    return Widget(val);
}

// Perfect forwarding
template<typename T>
void forwardToDetect(T&& arg) {
    detect(std::forward<T>(arg));
}

// ----------- EXTRA ADDITIONS -----------

// Print address (helps visualize moves)
void print_address(const Widget& w, const std::string& name) {
    std::cout << name << " address: " << &w << "\n";
}

// Demonstrate std::move vs std::forward
template<typename T>
void test_forwarding(T&& arg) {
    std::cout << "Using std::move → ";
    detect(std::move(arg));  // always rvalue

    std::cout << "Using std::forward → ";
    detect(std::forward<T>(arg)); // preserves category
}

// Temporary lifetime demo
void temporary_demo() {
    std::cout << "\nTemporary demo:\n";
    Widget temp = createWidget(99);
    std::cout << "Temp data: " << temp.data << "\n";
}

// Chain move operations
Widget chain_move(Widget w) {
    std::cout << "Inside chain_move\n";
    return w; // may trigger move or RVO
}

// ----------- MORE EXTRA ADDITIONS -----------

// Self-assignment safety test
void self_assignment_demo() {
    std::cout << "\nSelf-assignment demo:\n";
    Widget w(123);
    w = w; // copy self-assignment
}

// Emplace-like behavior simulation
template<typename... Args>
Widget make_widget(Args&&... args) {
    return Widget(std::forward<Args>(args)...);
}

// ------------------------------------

int main() {
    Widget w1(10);
    Widget w2 = w1;
    Widget w3 = std::move(w1);

    Widget w4(20);
    w4 = w2;
    w4 = std::move(w3);

    consume(std::move(w4));

    // -------- NEW FEATURE USAGE --------

    Widget w5(50);

    detect(w5);
    detect(std::move(w5));
    detect(createWidget(60));

    forwardToDetect(w2);
    forwardToDetect(createWidget(70));

    // -------- EXTRA FEATURE USAGE --------

    std::cout << "\n--- Address Tracking ---\n";
    print_address(w2, "w2");
    print_address(w5, "w5");

    std::cout << "\n--- Forwarding Test ---\n";
    test_forwarding(w2);                 // lvalue
    test_forwarding(createWidget(80));  // rvalue

    temporary_demo();

    std::cout << "\n--- Chain Move ---\n";
    Widget w6 = chain_move(createWidget(200));
    std::cout << "w6 data: " << w6.data << "\n";

    // -------- MORE EXTRA USAGE --------

    self_assignment_demo();

    std::cout << "\n--- Emplace-style Construction ---\n";
    Widget w7 = make_widget(300);
    std::cout << "w7 data: " << w7.data << "\n";

    // ----------------------------------

    return 0;
}
