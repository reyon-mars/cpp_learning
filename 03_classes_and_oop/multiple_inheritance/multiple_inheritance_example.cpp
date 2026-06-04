#include <iostream>
#include <typeinfo>
#include <memory>
#include <cassert>
#include <cstdint>

class A {
public:
    A()          { std::cout << "A constructed\n"; }
    virtual ~A() { std::cout << "A destroyed\n";   }

    virtual void method() const { std::cout << "A::method\n"; }
    virtual void info()   const { std::cout << "A info\n";    }

    void identify() const { std::cout << "I am A\n"; }
};

class B : virtual public A {
public:
    B()  { std::cout << "B constructed\n"; }
    ~B() { std::cout << "B destroyed\n";   }

    void method() const override { std::cout << "B::method\n"; }
    void info()   const override { std::cout << "B info\n";    }

    void identifyB() const { std::cout << "I am B\n"; }
};

class C : virtual public A {
public:
    C()  { std::cout << "C constructed\n"; }
    ~C() { std::cout << "C destroyed\n";   }

    void method() const override { std::cout << "C::method\n"; }
    void info()   const override { std::cout << "C info\n";    }

    void identifyC() const { std::cout << "I am C\n"; }
};

class D final : public B, public C {
public:
    D() : A{}, B{}, C{} { std::cout << "D constructed\n"; }
    ~D()                 { std::cout << "D destroyed\n";   }

    void method() const override final { std::cout << "D::method\n"; }
    void info()   const override       { std::cout << "D info\n";    }

    void callBaseVersions() const {
        B::method();
        C::method();
        A::method();
    }

    void identifyAll() const {
        identify();
        identifyB();
        identifyC();
        std::cout << "I am D\n";
    }

    void printObjectLayout() const {
        auto addr = [](const void* p) { return reinterpret_cast<std::uintptr_t>(p); };
        std::cout << "D:  0x" << std::hex << addr(this)                    << "\n"
                  << "B:  0x" << addr(static_cast<const B*>(this))         << "\n"
                  << "C:  0x" << addr(static_cast<const C*>(this))         << "\n"
                  << "A:  0x" << addr(static_cast<const A*>(this))         << std::dec << "\n";
    }

    void printSizes() const {
        std::cout << "sizeof A=" << sizeof(A)
                  << " B="       << sizeof(B)
                  << " C="       << sizeof(C)
                  << " D="       << sizeof(D) << "\n"
                  << "alignof D=" << alignof(D) << "\n";
    }

    void runtimeTypeInfo(const A& ref) const {
        std::cout << "RTTI: " << typeid(ref).name() << "\n";
    }
};

void callInfo(const A& obj) {
    obj.info();
}

void smartPointerDemo() {
    auto ptr = std::make_unique<D>();
    assert(ptr != nullptr);
    std::cout << "Smart pointer dispatch: ";
    ptr->method();
}

void referenceCastDemo(A& obj) {
    try {
        D& dref = dynamic_cast<D&>(obj);
        std::cout << "dynamic_cast<D&> succeeded: ";
        dref.method();
    } catch (const std::bad_cast& e) {
        std::cout << "dynamic_cast<D&> failed: " << e.what() << "\n";
    }
}

int main() {
    D obj;

    std::cout << "\n--- Direct call ---\n";
    obj.method();

    std::cout << "\n--- Through A& ---\n";
    A& aRef = obj;
    aRef.method();

    std::cout << "\n--- Through B* ---\n";
    static_cast<B*>(&obj)->method();

    std::cout << "\n--- Through C* ---\n";
    static_cast<C*>(&obj)->method();

    std::cout << "\n--- Explicit base calls ---\n";
    obj.callBaseVersions();

    std::cout << "\n--- Single A subobject (virtual inheritance) ---\n";
    std::cout << "A* via B == A* via C: " << std::boolalpha
              << (static_cast<A*>(static_cast<B*>(&obj)) ==
                  static_cast<A*>(static_cast<C*>(&obj))) << "\n";

    std::cout << "\n--- Identity ---\n";
    obj.identifyAll();

    std::cout << "\n--- Object layout ---\n";
    obj.printObjectLayout();

    std::cout << "\n--- Sizes & alignment ---\n";
    obj.printSizes();

    std::cout << "\n--- RTTI ---\n";
    obj.runtimeTypeInfo(obj);

    std::cout << "\n--- dynamic_cast<D*> ---\n";
    A* basePtr = &obj;
    if (D* dp = dynamic_cast<D*>(basePtr)) {
        std::cout << "Cast succeeded: ";
        dp->method();
    }

    std::cout << "\n--- Virtual info() dispatch ---\n";
    callInfo(obj);

    std::cout << "\n--- Smart pointer ---\n";
    smartPointerDemo();

    std::cout << "\n--- Reference cast ---\n";
    referenceCastDemo(obj);

    return 0;
}
