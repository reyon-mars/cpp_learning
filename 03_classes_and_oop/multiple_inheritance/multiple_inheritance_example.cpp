// Multiple Inheritance Exercise
// Diamond problem, virtual inheritance

#include <iostream>

class A {
public:
    A() { std::cout << "A constructed\n"; }
    virtual ~A() { std::cout << "A destroyed\n"; }   // 🔹 added
    virtual void method() { std::cout << "A::method\n"; }
};

class B : virtual public A {
public:
    B() { std::cout << "B constructed\n"; }
    ~B() { std::cout << "B destroyed\n"; }           // 🔹 added
    void method() override { std::cout << "B::method\n"; }
};

class C : virtual public A {
public:
    C() { std::cout << "C constructed\n"; }
    ~C() { std::cout << "C destroyed\n"; }           // 🔹 added
    void method() override { std::cout << "C::method\n"; }
};

class D : public B, public C {
public:
    D() { std::cout << "D constructed\n"; }
    ~D() { std::cout << "D destroyed\n"; }           // 🔹 added

    void method() override { std::cout << "D::method\n"; }

    void callBaseVersions() {
        B::method();
        C::method();
        A::method();
    }
};

int main() {

    D obj;

    std::cout << "---- Direct call ----\n";
    obj.method();

    std::cout << "---- Through A reference ----\n";
    A& a = obj;
    a.method();

    std::cout << "---- Through B pointer ----\n";   // 🔹 added
    B* b = &obj;
    b->method();

    std::cout << "---- Through C pointer ----\n";   // 🔹 added
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

    return 0;
}
