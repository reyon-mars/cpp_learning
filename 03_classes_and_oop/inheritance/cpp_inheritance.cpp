#include <iostream>
#include <typeinfo>
#include <memory>
#include <string_view>

class Super {
public:
    Super() noexcept : storage_{0}, accessible_storage_{0} {}
    explicit Super(int val) noexcept : storage_{val}, accessible_storage_{val} {}

    virtual ~Super() { std::cout << "Super destructor\n"; }

    void set(int val) noexcept   { storage_ = accessible_storage_ = val; }
    void reset() noexcept        { storage_ = accessible_storage_ = 0;   }
    [[nodiscard]] int get() const noexcept { return storage_; }

    virtual void info() const { std::cout << "Super\n"; }

protected:
    int accessible_storage_;

private:
    int storage_;
};

class Base {
public:
    Base() noexcept : storage_{0} {}
    explicit Base(int val) noexcept : storage_{val} {}

    virtual ~Base() { std::cout << "Base destructor\n"; }

    void set(int val) noexcept   { storage_ = val; }
    void reset() noexcept        { storage_ = 0;   }
    [[nodiscard]] int get() const noexcept { return storage_; }

    virtual void info() const { std::cout << "Base\n"; }

protected:
    int storage_;
};

class Sub final : public Super {
public:
    void print() const {
        std::cout << "Storage = " << accessible_storage_ << "\n";
    }

    void increment() noexcept { ++accessible_storage_; }

    void info() const override { std::cout << "Sub (derives Super)\n"; }

    void showAddress() const {
        std::cout << "Sub object address: " << static_cast<const void*>(this) << "\n";
    }
};

class SubMulti final : public Super, public Base {
public:
    SubMulti() noexcept = default;
    SubMulti(int superVal, int baseVal) noexcept
        : Super{superVal}, Base{baseVal} {}

    void print() const {
        std::cout << "Storage Super = " << Super::accessible_storage_ << "\n"
                  << "Storage Base  = " << Base::storage_             << "\n";
    }

    [[nodiscard]] int totalStorage() const noexcept {
        return Super::accessible_storage_ + Base::storage_;
    }

    void setBoth(int a, int b) noexcept {
        Super::set(a);
        Base::set(b);
    }

    void resetBoth() noexcept {
        Super::reset();
        Base::reset();
    }

    void showInfo() const {
        Super::info();
        Base::info();
    }

    void incrementBoth() noexcept {
        ++Super::accessible_storage_;
        ++Base::storage_;
    }

    void printAddresses() const {
        std::cout << "Address Super part: "
                  << static_cast<const void*>(static_cast<const Super*>(this)) << "\n"
                  << "Address Base part:  "
                  << static_cast<const void*>(static_cast<const Base*>(this))  << "\n";
    }

    void info() const override { std::cout << "SubMulti (multiple inheritance)\n"; }
};

void polymorphismDemo(Super& obj) {
    std::cout << "Virtual dispatch: ";
    obj.info();
}

void typeCheck(Super& obj) {
    std::cout << "Type check: ";
    if (dynamic_cast<Sub*>(&obj)) {
        std::cout << "Sub\n";
    } else {
        std::cout << "not Sub\n";
    }
}

void showRuntimeType(const Super& obj) {
    std::cout << "Runtime type: " << typeid(obj).name() << "\n";
}

void smartPointerDemo() {
    auto ptr = std::make_unique<Sub>();
    std::cout << "Smart pointer dispatch: ";
    ptr->info();
}

int main() {
    SubMulti object;
    object.Super::set(100);
    object.Base::set(50);
    object.Super::set(object.Super::get() + 1);
    object.print();
    std::cout << "Total Storage = " << object.totalStorage() << "\n";

    object.resetBoth();
    std::cout << "\nAfter reset:\n";
    object.print();

    std::cout << "\n--- setBoth ---\n";
    SubMulti obj2{10, 20};
    obj2.print();
    obj2.setBoth(30, 40);
    std::cout << "After setBoth:\n";
    obj2.print();
    std::cout << "Super::get = " << obj2.Super::get() << "\n"
              << "Base::get  = " << obj2.Base::get()  << "\n";
    obj2.showInfo();

    std::cout << "\n--- incrementBoth ---\n";
    obj2.incrementBoth();
    obj2.print();
    obj2.printAddresses();

    std::cout << "\n--- Polymorphism ---\n";
    Sub s;
    polymorphismDemo(s);

    std::cout << "\n--- Type Check ---\n";
    typeCheck(s);

    std::cout << "\n--- SubMulti info ---\n";
    obj2.info();

    std::cout << "\n--- Reference polymorphism ---\n";
    polymorphismDemo(s);

    std::cout << "\n--- Runtime Type ---\n";
    showRuntimeType(s);

    std::cout << "\n--- Smart Pointer ---\n";
    smartPointerDemo();

    std::cout << "\n--- Sub features ---\n";
    s.increment();
    s.print();
    s.showAddress();

    return 0;
}
