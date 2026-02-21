// Multiple Inheritance Exercise
// Diamond problem, virtual inheritance

#include <iostream>

class A {
public:
    virtual void method() { std::cout << "A::method\n"; }
};

class B : virtual public A {
public:
    void method() override { std::cout << "B::method\n"; }
};

class C : virtual public A {
public:
    void method() override { std::cout << "C::method\n"; }
};

class D : public B, public C {
public:
    void method() override { std::cout << "D::method\n"; }
};

int main() {
    D obj;
    obj.method();
    
    A& a = obj;
    a.method();
    
    return 0;
}
