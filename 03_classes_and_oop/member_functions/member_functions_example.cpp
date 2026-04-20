// Member Functions Exercise
// const, non-const, ref-qualified, virtual member functions

#include <iostream>
#include <utility>

class MyClass {
private:
    int value;
    
public:
    MyClass(int v) : value(v) {}

    // -------- EXTRA ADDITIONS (Rule of 5 basics) --------
    MyClass(const MyClass&) = default;
    MyClass(MyClass&&) noexcept = default;
    MyClass& operator=(const MyClass&) = default;
    MyClass& operator=(MyClass&&) noexcept = default;
    ~MyClass() = default;
    // ---------------------------------------------------
    
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

    // const-qualified chaining (returns new object)
    MyClass addConst(int v) const {
        return MyClass(value + v);
    }
    // --------------------------------
    
    // Virtual member function
    virtual void display() const {
        std::cout << "Value: " << value << "\n";
    }

    // -------- EXTRA ADDITION --------
    [[nodiscard]] int safeGet() const {
        return value;
    }
    // --------------------------------

    // -------- EXTRA ADDITION --------
    static void classInfo() {
        std::cout << "MyClass static info function\n";
    }
    // --------------------------------
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
    obj.process();                 
    std::move(obj).process();      

    // -------- NEW FEATURE USAGE --------

    const MyClass cobj(300);
    cobj.process();                     
    std::move(cobj).process();          

    // Method chaining
    obj.add(10).add(20).multiply(2);
    std::cout << "After chaining: " << obj.getValue() << "\n";

    obj.safePrint();

    std::cout << "Is positive? "
              << (obj.isPositive() ? "Yes\n" : "No\n");

    FinalDerived fd(500);
    fd.display();

    showObject(obj);

    // -------- EXTRA USAGE --------

    std::cout << "\n--- Const chaining demo ---\n";
    MyClass newObj = cobj.addConst(50);
    std::cout << "New object value: " << newObj.getValue() << "\n";

    std::cout << "\n--- [[nodiscard]] demo ---\n";
    int safeVal = obj.safeGet();
    std::cout << "Safe value retrieved: " << safeVal << "\n";

    std::cout << "\n--- Static function demo ---\n";
    MyClass::classInfo();

    // ----------------------------------

    // 🔹 Virtual dispatch demonstration
    MyClass* basePtr = new Derived(200);
    basePtr->display();

    delete basePtr;

    return 0;
}
