// Multiple Inheritance Exercise
// Diamond problem, virtual inheritance

#include <iostream>
#include <typeinfo>

class A {
public:
    A() { std::cout << "A constructed\n"; }
    virtual ~A() { std::cout << "A destroyed\n"; }
    virtual void method() { std::cout << "A::method\n"; }

    void identify() { std::cout << "I am A\n"; }
};

class B : virtual public A {
public:
    B() { std::cout << "B constructed\n"; }
    ~B() { std::cout << "B destroyed\n"; }
    void method() override { std::cout << "B::method\n"; }

    void identifyB() { std::cout << "I am B\n"; }
};

class C : virtual public A {
public:
    C() { std::cout << "C constructed\n"; }
    ~C() { std::cout << "C destroyed\n"; }
    void method() override { std::cout << "C::method\n"; }

    void identifyC() { std::cout << "I am C\n"; }
};

class D : public B, public C {
public:
    // ✅ Explicit virtual base initialization
    D() : A() { std::cout << "D constructed\n"; }

    ~D() { std::cout << "D destroyed\n"; }

    // ✅ final override
    void method() override final { std::cout << "D::method\n"; }

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
        std::cout << "Address as B: " << static_cast<B*>(this) << "\n";
        std::cout << "Address as C: " << static_cast<C*>(this) << "\n";
        std::cout << "Address as A: " << static_cast<A*>(this) << "\n";
    }

    // -------- EXTRA ADDITIONS --------

    void printSizes() {
        std::cout << "Size of A: " << sizeof(A) << "\n";
        std::cout << "Size of B: " << sizeof(B) << "\n";
        std::cout << "Size of C: " << sizeof(C) << "\n";
        std::cout << "Size of D: " << sizeof(D) << "\n";
    }

    void runtimeTypeInfo(A* ptr) {
        std::cout << "RTTI type: " << typeid(*ptr).name() << "\n";
    }

    // --------------------------------
};

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

    // ----------------------------------

    return 0;
}
