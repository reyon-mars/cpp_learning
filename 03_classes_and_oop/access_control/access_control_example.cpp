#include <iostream>
#include <string>
#include <string_view>
#include <memory>
#include <cassert>

class Friend;
void friend_function();

class Base {
public:
    int pub_member = 0;

    void print() const {
        std::cout << "pub: "  << pub_member
                  << " prot: " << prot_member
                  << " priv: " << priv_member << "\n";
    }

    [[nodiscard]] int getPrivate() const noexcept { return priv_member; }
    void setPrivate(int val) noexcept              { priv_member = val;  }

protected:
    int prot_member = 0;

private:
    int priv_member = 0;

    friend class Friend;
    friend void friend_function();
};

class Derived : public Base {
public:
    void access() {
        pub_member  = 1;
        prot_member = 2;
    }
};

class AdvancedDerived : public Derived {
public:
    void advancedAccess() {
        pub_member  = 11;
        prot_member = 22;
    }
};

class Friend {
public:
    void access(Base& obj) noexcept {
        obj.priv_member = 10;
    }
};

void friend_function() {
    Base obj;
    obj.priv_member = 20;
    obj.print();
}

class ProtectedDerived : protected Base {
public:
    void test() {
        pub_member  = 5;
        prot_member = 6;
    }
};

class PrivateDerived : private Base {
public:
    void test() {
        pub_member  = 7;
        prot_member = 8;
    }
};

void show_state(const Base& obj, std::string_view label) {
    std::cout << "[" << label << "] ";
    obj.print();
}

void modify_public(Base& obj) noexcept {
    obj.pub_member += 5;
}

class Counter {
public:
    static inline int count = 0;
    Counter() noexcept { ++count; }
};

class Shape {
public:
    virtual void draw() const { std::cout << "Drawing Shape\n"; }
    virtual ~Shape() = default;
};

class Circle final : public Shape {
public:
    void draw() const override { std::cout << "Drawing Circle\n"; }
};

void slicing_demo(Base obj) {
    std::cout << "Sliced Base copy: ";
    obj.print();
}

class Container {
public:
    class Nested {
    public:
        void show() const { std::cout << "Nested inside Container\n"; }
    };

    [[nodiscard]] int getSecret() const noexcept { return secret_; }

private:
    int secret_ = 500;
};

class ConstDemo {
public:
    explicit ConstDemo(int v) noexcept : value_{v} {}
    [[nodiscard]] int getValue() const noexcept { return value_; }

private:
    int value_;
};

class Inspector {
public:
    static void inspect(const Base& obj) {
        std::cout << "Inspector (via getter): " << obj.getPrivate() << "\n";
    }
};

int main() {
    Base obj;
    obj.pub_member = 5;
    obj.print();

    std::cout << "\n--- Derived public access ---\n";
    Derived d;
    d.access();
    d.print();

    std::cout << "\n--- Friend class ---\n";
    Friend f;
    f.access(obj);
    obj.print();

    std::cout << "\n--- Friend function ---\n";
    friend_function();

    std::cout << "\n--- Protected/Private inheritance ---\n";
    ProtectedDerived pd;
    pd.test();

    PrivateDerived pr;
    pr.test();

    std::cout << "\n--- setPrivate / getter ---\n";
    obj.setPrivate(99);
    show_state(obj, "after setPrivate(99)");
    std::cout << "getPrivate: " << obj.getPrivate() << "\n";

    std::cout << "\n--- modify_public ---\n";
    modify_public(obj);
    show_state(obj, "after modify_public");

    std::cout << "\n--- Static member ---\n";
    Counter c1, c2, c3;
    std::cout << "Counter::count: " << Counter::count << "\n";

    std::cout << "\n--- Polymorphism (unique_ptr) ---\n";
    std::unique_ptr<Shape> shape = std::make_unique<Circle>();
    shape->draw();

    std::cout << "\n--- Object slicing ---\n";
    slicing_demo(d);

    std::cout << "\n--- setPrivate(123) ---\n";
    obj.setPrivate(123);
    obj.print();

    std::cout << "\n--- Nested class ---\n";
    Container::Nested nested;
    nested.show();
    Container container;
    std::cout << "Container secret: " << container.getSecret() << "\n";

    std::cout << "\n--- Const member function ---\n";
    const ConstDemo cd{77};
    std::cout << "Const value: " << cd.getValue() << "\n";

    std::cout << "\n--- AdvancedDerived ---\n";
    AdvancedDerived ad;
    ad.advancedAccess();
    ad.print();

    std::cout << "\n--- Inspector ---\n";
    Inspector::inspect(obj);

    assert(obj.getPrivate() == 123);

    return 0;
}
