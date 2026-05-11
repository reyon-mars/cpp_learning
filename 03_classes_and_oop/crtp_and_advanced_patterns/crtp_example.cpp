```cpp
#include <iostream>
#include <type_traits> // ✅ ADDED

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

    // ----------- EXTRA ADDITIONS -----------

    // Compile-time information
    void typeInfo() {
        std::cout << "CRTP type size: "
                  << sizeof(Derived) << " bytes\n";
    }

    // Static polymorphism check
    void staticDispatch() {
        std::cout << "[Base] Static dispatch working\n";
        static_cast<Derived*>(this)->implementation();
    }

    // --------------------------------------

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

    // ----------- EXTRA ADDITION -----------
    void uniqueTask() {
        std::cout << "Derived unique task executed\n";
    }
    // -------------------------------------
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

    // ----------- EXTRA ADDITION -----------
    void uniqueTask() {
        std::cout << "AnotherDerived unique task executed\n";
    }
    // -------------------------------------
};

// ----------- NEW ADDITION -----------

// Generic function using CRTP base
template<typename T>
void runCRTP(Base<T>& obj) {
    std::cout << "[Generic] Running CRTP interface\n";
    obj.interface();
}

// ------------------------------------

// ----------- EXTRA ADDITIONS -----------

// Compile-time checker
template<typename T>
void checkCRTP() {
    std::cout << "Is class type? "
              << std::is_class<T>::value << "\n";
}

// Generic helper
template<typename T>
void executeExtra(T& obj) {
    std::cout << "[Generic] Executing extra feature\n";
    obj.extraFeature();
}

// --------------------------------------

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

    // ----------- EXTRA USAGE -----------
    d.typeInfo();
    d.staticDispatch();
    d.uniqueTask();
    // -----------------------------------

    std::cout << "------------------\n";

    ad.interface();
    ad.common();
    ad.callDerivedTwice();    // new usage
    ad.extraFeature();        // new usage

    // ----------- EXTRA USAGE -----------
    ad.typeInfo();
    ad.staticDispatch();
    ad.uniqueTask();
    // -----------------------------------

    std::cout << "------------------\n";

    // Generic CRTP usage
    runCRTP(d);
    runCRTP(ad);

    std::cout << "------------------\n";

    // ----------- MORE EXTRA USAGE -----------

    checkCRTP<Derived>();
    checkCRTP<AnotherDerived>();

    executeExtra(d);
    executeExtra(ad);

    // Compile-time checks
    static_assert(std::is_base_of<Base<Derived>, Derived>::value,
                  "Derived must inherit from Base<Derived>");

    static_assert(std::is_base_of<Base<AnotherDerived>, AnotherDerived>::value,
                  "AnotherDerived must inherit from Base<AnotherDerived>");

    // ----------------------------------------

    return 0;
}
```
