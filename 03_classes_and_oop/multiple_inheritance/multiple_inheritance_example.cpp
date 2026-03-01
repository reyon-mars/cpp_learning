// Multiple Inheritance Exercise
// Diamond problem, virtual inheritance

#include <iostream>

class A {
public:
    A() { std::cout << "A constructed\n"; }  // 🔹 added
    virtual void method() { std::cout << "A::method\n"; }
};

class B : virtual public A {
public:
    B() { std::cout << "B constructed\n"; }  // 🔹 added
    void method() override { std::cout << "B::method\n"; }
};

class C : virtual public A {
public:
    C() { std::cout << "C constructed\n"; }  // 🔹 added
    void method() override { std::cout << "C::method\n"; }
};

class D : public B, public C {
public:
    D() { std::cout << "D constructed\n"; }  // 🔹 added

    void method() override { std::cout << "D::method\n"; }

    void callBaseVersions() {               // 🔹 added helper
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
    a.method();   // virtual dispatch → D::method

    std::cout << "---- Explicit base calls ----\n";
    obj.callBaseVersions();

    // 🔹 Proving single A instance (virtual inheritance)
    B* bPtr = &obj;
    C* cPtr = &obj;

    std::cout << "Address of A via B: "
              << static_cast<A*>(bPtr) << "\n";

    std::cout << "Address of A via C: "
              << static_cast<A*>(cPtr) << "\n";

    return 0;
}
