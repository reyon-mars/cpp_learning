// Rvalue References and Value Categories Exercise
// lvalue, rvalue, xvalue semantics

#include <iostream>
#include <utility>

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
        other.data = 0; // small safety addition
    }

    // ✅ added copy assignment
    Widget& operator=(const Widget& other) {
        std::cout << "Copy assignment\n";
        data = other.data;
        return *this;
    }

    // ✅ added move assignment
    Widget& operator=(Widget&& other) noexcept {
        std::cout << "Move assignment\n";
        data = other.data;
        other.data = 0;
        return *this;
    }
};

// ✅ small helper to show rvalue reference
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

// Perfect forwarding example
template<typename T>
void forwardToDetect(T&& arg) {
    detect(std::forward<T>(arg));
}

// ------------------------------------

int main() {
    Widget w1(10);              // lvalue
    Widget w2 = w1;             // Copy constructor
    Widget w3 = std::move(w1);  // Move constructor

    Widget w4(20);
    w4 = w2;                    // Copy assignment
    w4 = std::move(w3);         // Move assignment

    consume(std::move(w4));     // rvalue reference usage

    // -------- NEW FEATURE USAGE --------

    Widget w5(50);

    detect(w5);                 // lvalue
    detect(std::move(w5));      // rvalue

    detect(createWidget(60));   // temporary → rvalue

    forwardToDetect(w2);        // preserves lvalue
    forwardToDetect(createWidget(70)); // preserves rvalue

    // ----------------------------------

    return 0;
}
