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

    // ----------- NEW ADDITION -----------
    void callDerivedTwice() {
        std::cout << "[Base] Calling implementation twice:\n";
        static_cast<Derived*>(this)->implementation();
        static_cast<Derived*>(this)->implementation();
    }
    // -----------------------------------

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

    // ----------- NEW ADDITION -----------
    void extraFeature() {
        std::cout << "Derived extra feature\n";
    }
    // -----------------------------------
};

// ======================================================
// Second Derived (reuse without virtuals)
// ======================================================
class AnotherDerived : public Base<AnotherDerived> {
public:
    void implementation() {
        std::cout << "AnotherDerived implementation\n";
    }

    // ----------- NEW ADDITION -----------
    void extraFeature() {
        std::cout << "AnotherDerived extra feature\n";
    }
    // -----------------------------------
};

// ----------- NEW ADDITION -----------

// Generic function using CRTP base
template<typename T>
void runCRTP(Base<T>& obj) {
    std::cout << "[Generic] Running CRTP interface\n";
    obj.interface();
}

// ------------------------------------

// ======================================================
// MAIN
// ======================================================
int main() {
    Derived d;
    AnotherDerived ad;

    d.interface();
    d.common();
    d.callDerivedTwice();     // new usage
    d.extraFeature();         // new usage

    std::cout << "------------------\n";

    ad.interface();
    ad.common();
    ad.callDerivedTwice();    // new usage
    ad.extraFeature();        // new usage

    std::cout << "------------------\n";

    // Generic CRTP usage
    runCRTP(d);
    runCRTP(ad);

    return 0;
}
