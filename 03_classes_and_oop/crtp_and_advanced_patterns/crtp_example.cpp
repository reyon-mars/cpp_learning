#include <iostream>
#include <type_traits>
#include <concepts>

template<typename Derived>
class Base {
public:
    void interface() {
        pre();
        self().implementation();
        post();
    }

    void common() const {
        std::cout << "Common CRTP behavior\n";
    }

    void callDerivedTwice() {
        std::cout << "[Base] Calling implementation twice:\n";
        self().implementation();
        self().implementation();
    }

    void typeInfo() const {
        std::cout << "CRTP Derived size: " << sizeof(Derived) << " bytes\n";
    }

    void staticDispatch() {
        std::cout << "[Base] Static dispatch: ";
        self().implementation();
    }

private:
    [[nodiscard]] Derived& self() noexcept {
        return *static_cast<Derived*>(this);
    }

    [[nodiscard]] const Derived& self() const noexcept {
        return *static_cast<const Derived*>(this);
    }

    void pre()  const { std::cout << "[Base] Before implementation\n"; }
    void post() const { std::cout << "[Base] After implementation\n";  }
};

template<typename T>
concept CRTPDerived = requires(T obj) {
    { obj.implementation() };
    { obj.extraFeature()   };
    { obj.uniqueTask()     };
};

class Derived : public Base<Derived> {
public:
    void implementation() const { std::cout << "Derived::implementation\n";  }
    void extraFeature()   const { std::cout << "Derived::extraFeature\n";    }
    void uniqueTask()     const { std::cout << "Derived::uniqueTask\n";      }
};

class AnotherDerived : public Base<AnotherDerived> {
public:
    void implementation() const { std::cout << "AnotherDerived::implementation\n"; }
    void extraFeature()   const { std::cout << "AnotherDerived::extraFeature\n";   }
    void uniqueTask()     const { std::cout << "AnotherDerived::uniqueTask\n";     }
};

template<typename T>
void runCRTP(Base<T>& obj) {
    std::cout << "[Generic] runCRTP:\n";
    obj.interface();
}

template<typename T>
void checkCRTP() {
    std::cout << "Is class type: " << std::is_class_v<T> << "\n";
    static_assert(std::is_base_of_v<Base<T>, T>,
                  "T must publicly inherit Base<T>");
}

template<CRTPDerived T>
void executeExtra(const T& obj) {
    std::cout << "[Generic] extraFeature: ";
    obj.extraFeature();
}

template<CRTPDerived T>
void runAll(T& obj) {
    obj.interface();
    obj.common();
    obj.callDerivedTwice();
    obj.extraFeature();
    obj.typeInfo();
    obj.staticDispatch();
    obj.uniqueTask();
}

int main() {
    Derived       d;
    AnotherDerived ad;

    std::cout << "=== Derived ===\n";
    runAll(d);

    std::cout << "\n=== AnotherDerived ===\n";
    runAll(ad);

    std::cout << "\n=== Generic runCRTP ===\n";
    runCRTP(d);
    runCRTP(ad);

    std::cout << "\n=== checkCRTP ===\n";
    checkCRTP<Derived>();
    checkCRTP<AnotherDerived>();

    std::cout << "\n=== executeExtra ===\n";
    executeExtra(d);
    executeExtra(ad);

    return 0;
}
