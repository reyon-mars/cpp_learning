// Multiple Inheritance Exercise
// Diamond problem, virtual inheritance

#include <iostream>
#include <typeinfo>
#include <memory>      // ✅ ADDED
#include <cassert>     // ✅ ADDED
#include <iomanip>     // ✅ ADDED

class A {
public:
    A() { std::cout << "A constructed\n"; }
    virtual ~A() { std::cout << "A destroyed\n"; }

    virtual void method() {
        std::cout << "A::method\n";
    }

    void identify() {
        std::cout << "I am A\n";
    }

    // -------- NEW ADDITION --------
    virtual void info() const {
        std::cout << "Base class A info\n";
    }
    // --------------------------------
};

class B : virtual public A {
public:
    B() { std::cout << "B constructed\n"; }

    ~B() {
        std::cout << "B destroyed\n";
    }

    void method() override {
        std::cout << "B::method\n";
    }

    void identifyB() {
        std::cout << "I am B\n";
    }

    // -------- NEW ADDITION --------
    void info() const override {
        std::cout << "Derived class B info\n";
    }
    // --------------------------------
};

class C : virtual public A {
public:
    C() { std::cout << "C constructed\n"; }

    ~C() {
        std::cout << "C destroyed\n";
    }

    void method() override {
        std::cout << "C::method\n";
    }

    void identifyC() {
        std::cout << "I am C\n";
    }

    // -------- NEW ADDITION --------
    void info() const override {
        std::cout << "Derived class C info\n";
    }
    // --------------------------------
};

class D : public B, public C {
public:
    // ✅ Explicit virtual base initialization
    D() : A() {
        std::cout << "D constructed\n";
    }

    ~D() {
        std::cout << "D destroyed\n";
    }

    // ✅ final override
    void method() override final {
        std::cout << "D::method\n";
    }

    void callBaseVersions() {
        B::method();
        C::method();
        A::method();
    }

    void identifyAll() {
        identify();
        identifyB();
        identifyC();
        std::cout << "I am D\n";
    }

    void printObjectLayout() {
        std::cout << "Address of D: " << this << "\n";

        std::cout << "Address as B: "
                  << static_cast<B*>(this) << "\n";

        std::cout << "Address as C: "
                  << static_cast<C*>(this) << "\n";

        std::cout << "Address as A: "
                  << static_cast<A*>(this) << "\n";
    }

    // -------- EXTRA ADDITIONS --------

    void printSizes() {
        std::cout << "Size of A: "
                  << sizeof(A) << "\n";

        std::cout << "Size of B: "
                  << sizeof(B) << "\n";

        std::cout << "Size of C: "
                  << sizeof(C) << "\n";

        std::cout << "Size of D: "
                  << sizeof(D) << "\n";
    }

    void runtimeTypeInfo(A* ptr) {
        std::cout << "RTTI type: "
                  << typeid(*ptr).name() << "\n";
    }

    // -------- MORE ADDITIONS --------

    void info() const override {
        std::cout << "Most derived class D info\n";
    }

    void showAlignment() {
        std::cout << "Alignment of D: "
                  << alignof(D) << "\n";
    }

    void printVptrHint() {
        std::cout << "Object address: "
                  << this << "\n";
    }

    // --------------------------------
};

// -------- GLOBAL HELPER ADDITIONS --------

// Polymorphism helper
void callInfo(const A& obj) {
    obj.info();
}

// Safe smart pointer demo
void smartPointerDemo() {
    std::unique_ptr<A> ptr = std::make_unique<D>();

    std::cout << "\nSmart pointer polymorphism:\n";
    ptr->method();

    assert(ptr != nullptr);
}

// Reference cast demo
void referenceCastDemo(A& obj) {
    try {
        D& dref = dynamic_cast<D&>(obj);

        std::cout << "Reference cast successful\n";
        dref.method();
    }
    catch (const std::bad_cast& e) {
        std::cout << "Reference cast failed: "
                  << e.what() << "\n";
    }
}

// -----------------------------------------

int main() {

    D obj;

    std::cout << "---- Direct call ----\n";
    obj.method();

    std::cout << "---- Through A reference ----\n";
    A& a = obj;
    a.method();

    std::cout << "---- Through B pointer ----\n";
    B* b = &obj;
    b->method();

    std::cout << "---- Through C pointer ----\n";
    C* c = &obj;
    c->method();

    std::cout << "---- Explicit base calls ----\n";
    obj.callBaseVersions();

    B* bPtr = &obj;
    C* cPtr = &obj;

    std::cout << "Address of A via B: "
              << static_cast<A*>(bPtr) << "\n";

    std::cout << "Address of A via C: "
              << static_cast<A*>(cPtr) << "\n";

    // -------- EXTRA USAGE --------

    std::cout << "---- Identity check ----\n";
    obj.identifyAll();

    std::cout << "---- Object layout ----\n";
    obj.printObjectLayout();

    // -------- NEW ADVANCED USAGE --------

    std::cout << "---- Size Analysis ----\n";
    obj.printSizes();

    std::cout << "---- RTTI Demo ----\n";
    obj.runtimeTypeInfo(&obj);

    std::cout << "---- dynamic_cast Demo ----\n";

    A* basePtr = &obj;

    if (D* derivedPtr = dynamic_cast<D*>(basePtr)) {
        std::cout << "Successfully cast A* to D*\n";
    }

    // -------- MORE ADDED FEATURES --------

    std::cout << "---- Virtual info() dispatch ----\n";
    callInfo(obj);

    std::cout << "---- Alignment Demo ----\n";
    obj.showAlignment();

    std::cout << "---- Vptr Hint ----\n";
    obj.printVptrHint();

    std::cout << "---- Smart Pointer Demo ----\n";
    smartPointerDemo();

    std::cout << "---- Reference Cast Demo ----\n";
    referenceCastDemo(obj);

    std::cout << "---- Memory Layout Formatting ----\n";

    std::cout << std::hex << std::showbase;

    std::cout << "D object address: "
              << reinterpret_cast<std::uintptr_t>(&obj)
              << "\n";

    std::cout << std::dec;

    // ----------------------------------

    return 0;
}
