#include <iostream>
#include <new>
#include <type_traits>

// ---------------- ORIGINAL CODE (UNCHANGED) ----------------

class base {
private:
    int a;

public:
    base() { std::cout << "Constructor\n"; }
    ~base() { std::cout << "Destructor\n"; }
};

int main(void) {
    std::cout << "Normal case\n";
    base* obj = new base();
    delete obj;

    std::cout << "\nPlacement New Case\n";

    alignas(base) char memory[sizeof(base) * 2];

    base* obj1 = new (&memory[0]) base();
    base* obj2 = new (&memory[sizeof(base)]) base();

    obj1->~base();
    obj2->~base();

    // -------- SMALL ADDED USAGE --------
    test_derived_allocation();
    test_virtual_destructor();
    // ----------------------------------

    return 0;
}

// ----------------------------------------------------------
// SMALL EXTRA CODE (ADDED ONLY)
// ----------------------------------------------------------

// Derived class demo
class derived : public base {
public:
    derived() {
        std::cout << "Derived Constructor\n";
    }
    ~derived() {
        std::cout << "Derived Destructor\n";
    }
};

// Virtual destructor demo
class poly_base {
public:
    poly_base() {
        std::cout << "poly_base ctor\n";
    }
    virtual ~poly_base() {
        std::cout << "poly_base dtor\n";
    }
};

class poly_derived : public poly_base {
public:
    poly_derived() {
        std::cout << "poly_derived ctor\n";
    }
    ~poly_derived() override {
        std::cout << "poly_derived dtor\n";
    }
};

// Simple derived allocation test
void test_derived_allocation() {
    std::cout << "\n[Extra] Derived Allocation\n";
    derived* d = new derived();
    delete d;
}

// Virtual destructor test
void test_virtual_destructor() {
    std::cout << "\n[Extra] Virtual Destructor Test\n";
    poly_base* p = new poly_derived();
    delete p;
}
