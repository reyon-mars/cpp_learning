// Member Functions Exercise
// const, non-const, ref-qualified, virtual member functions

#include <iostream>
#include <utility>

class MyClass {
private:
    int value;
    
public:
    MyClass(int v) : value(v) {}
    
    // Non-const member function
    void setValue(int v) { value = v; }
    
    // Const member function
    int getValue() const { return value; }
    
    // Ref-qualified member functions (C++11)
    void process() & { std::cout << "Lvalue reference\n"; }
    void process() && { std::cout << "Rvalue reference\n"; }
    
    // Virtual member function
    virtual void display() const {
        std::cout << "Value: " << value << "\n";
    }
};

// 🔹 Added derived class
class Derived : public MyClass {
public:
    Derived(int v) : MyClass(v) {}

    void display() const override {
        std::cout << "Derived display\n";
    }
};

int main() {
    MyClass obj(42);
    obj.setValue(100);

    std::cout << "Value: " << obj.getValue() << "\n";
    obj.display();

    // 🔹 Call ref-qualified functions
    obj.process();                 // lvalue
    std::move(obj).process();      // rvalue

    // 🔹 Virtual dispatch demonstration
    MyClass* basePtr = new Derived(200);
    basePtr->display();            // calls Derived::display

    delete basePtr;

    return 0;
}
