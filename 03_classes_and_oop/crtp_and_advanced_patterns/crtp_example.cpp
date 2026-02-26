#include <iostream>

// ======================================================
// CRTP Base
// ======================================================
template<typename Derived>
class Base {
public:
    void interface() {
        pre();
        static_cast<Derived*>(this)->implementation();
        post();
    }

    // 🔹 default behavior reusable by all derived classes
    void common() {
        std::cout << "Common CRTP behavior\n";
    }

private:
    void pre() {
        std::cout << "[Base] Before implementation\n";
    }

    void post() {
        std::cout << "[Base] After implementation\n";
    }
};

// ======================================================
// First Derived
// ======================================================
class Derived : public Base<Derived> {
public:
    void implementation() {
        std::cout << "Derived implementation\n";
    }
};

// ======================================================
// Second Derived (reuse without virtuals)
// ======================================================
class AnotherDerived : public Base<AnotherDerived> {
public:
    void implementation() {
        std::cout << "AnotherDerived implementation\n";
    }
};

// ======================================================
// MAIN
// ======================================================
int main() {
    Derived d;
    AnotherDerived ad;

    d.interface();
    d.common();

    std::cout << "------------------\n";

    ad.interface();
    ad.common();

    return 0;
}
