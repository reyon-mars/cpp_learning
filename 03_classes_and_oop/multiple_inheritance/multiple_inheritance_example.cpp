// Multiple Inheritance Exercise
// Diamond problem, virtual inheritance

#include <iostream>

class A {
public:
    A() { std::cout << "A constructed\n"; }
    virtual ~A() { std::cout << "A destroyed\n"; }
    virtual void method() { std::cout << "A::method\n"; }

    // -------- EXTRA ADDITION --------
    void identify() { std::cout << "I am A\n"; }
    // --------------------------------
};

class B : virtual public A {
public:
    B() { std::cout << "B constructed\n"; }
    ~B() { std::cout << "B destroyed\n"; }
    void method() override { std::cout << "B::method\n"; }

    // -------- EXTRA ADDITION --------
    void identifyB() { std::cout << "I am B\n"; }
    // --------------------------------
};

class C : virtual public A {
public:
    C() { std::cout << "C constructed\n"; }
    ~C() { std::cout << "C destroyed\n"; }
    void method() override { std::cout << "C::method\n"; }

    // -------- EXTRA ADDITION --------
    void identifyC() { std::cout << "I am C\n"; }
    // --------------------------------
};

class D : public B, public C {
public:
    D() { std::cout << "D constructed\n"; }
    ~D() { std::cout << "D destroyed\n"; }

    void method() override { std::cout << "D::method\n"; }

    void callBaseVersions() {
        B::method();
        C::method();
        A::method();
    }

    // -------- EXTRA ADDITIONS --------
    void identifyAll() {
        identify();   // from A
        identifyB();  // from B
        identifyC();  // from C
        std::cout << "I am D\n";
    }

    void printObjectLayout() {
        std::cout << "Address of D: " << this << "\n";
        std::cout << "Address as B: " << static_cast<B*>(this) << "\n";
        std::cout << "Address as C: " << static_cast<C*>(this) << "\n";
        std::cout << "Address as A: " << static_cast<A*>(this) << "\n";
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

    // ----------------------------------

    return 0;
}
