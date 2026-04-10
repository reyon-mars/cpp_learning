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

    // -------- NEW ADDITIONS --------
    void process() const & { std::cout << "Const lvalue reference\n"; }
    void process() const && { std::cout << "Const rvalue reference\n"; }

    // Method chaining
    MyClass& add(int v) {
        value += v;
        return *this;
    }

    // noexcept function
    void safePrint() const noexcept {
        std::cout << "Safe value: " << value << "\n";
    }

    // -------- EXTRA ADDITIONS --------
    MyClass& multiply(int v) {
        value *= v;
        return *this;
    }

    bool isPositive() const {
        return value > 0;
    }
    // --------------------------------
    
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

// -------- NEW ADDITION --------
class FinalDerived final : public MyClass {
public:
    FinalDerived(int v) : MyClass(v) {}

    void display() const override {
        std::cout << "FinalDerived display\n";
    }
};
// --------------------------------

// -------- EXTRA ADDITION --------
void showObject(const MyClass& obj) {
    std::cout << "[Helper] Value = " << obj.getValue() << "\n";
}
// --------------------------------

int main() {
    MyClass obj(42);
    obj.setValue(100);

    std::cout << "Value: " << obj.getValue() << "\n";
    obj.display();

    // 🔹 Call ref-qualified functions
    obj.process();                 // lvalue
    std::move(obj).process();      // rvalue

    // -------- NEW FEATURE USAGE --------

    const MyClass cobj(300);
    cobj.process();                     // const lvalue
    std::move(cobj).process();          // const rvalue

    // Method chaining
    obj.add(10).add(20).multiply(2);
    std::cout << "After chaining: " << obj.getValue() << "\n";

    // noexcept function
    obj.safePrint();

    std::cout << "Is positive? "
              << (obj.isPositive() ? "Yes\n" : "No\n");

    // Final class usage
    FinalDerived fd(500);
    fd.display();

    showObject(obj);

    // ----------------------------------

    // 🔹 Virtual dispatch demonstration
    MyClass* basePtr = new Derived(200);
    basePtr->display();            // calls Derived::display

    delete basePtr;

    return 0;
}
