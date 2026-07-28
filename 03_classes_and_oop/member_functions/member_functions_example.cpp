#include <algorithm>
#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <typeinfo>
#include <utility>
#include <vector>

class MyClass {
public:
    explicit MyClass(int v) noexcept : value_{v} {}

    MyClass(const MyClass&)            = default;
    MyClass(MyClass&&) noexcept        = default;
    MyClass& operator=(const MyClass&) = default;
    MyClass& operator=(MyClass&&) noexcept = default;
    virtual ~MyClass()                 = default;

    void setValue(int v) noexcept { value_ = v; }
    void reset()         noexcept { value_ = 0; }

    [[nodiscard]] int  getValue() const noexcept { return value_; }
    [[nodiscard]] int  safeGet()  const noexcept { return value_; }
    [[nodiscard]] bool isPositive() const noexcept { return value_ > 0; }

    void process()        &  { std::cout << "Lvalue\n";        }
    void process()        && { std::cout << "Rvalue\n";        }
    void process() const  &  { std::cout << "Const lvalue\n";  }
    void process() const  && { std::cout << "Const rvalue\n";  }

    MyClass& add(int v)      noexcept { value_ += v; return *this; }
    MyClass& multiply(int v) noexcept { value_ *= v; return *this; }

    [[nodiscard]] MyClass addConst(int v) const noexcept {
        return MyClass{value_ + v};
    }

    void safePrint() const noexcept {
        std::cout << "Safe value: " << value_ << "\n";
    }

    virtual void display() const {
        std::cout << "MyClass(" << value_ << ")\n";
    }

    [[nodiscard]] virtual bool equals(const MyClass& other) const {
        return typeid(*this) == typeid(other) && value_ == other.value_;
    }

    static void classInfo() {
        std::cout << "MyClass — static info\n";
    }

    [[nodiscard]] bool operator==(const MyClass&) const = default;

    [[nodiscard]] bool operator<(const MyClass& other) const noexcept {
        return value_ < other.value_;
    }

    friend std::ostream& operator<<(std::ostream& os, const MyClass& obj) {
        return os << "MyClass(" << obj.value_ << ")";
    }

private:
    int value_;
};

class Derived : public MyClass {
public:
    explicit Derived(int v) noexcept : MyClass{v} {}

    void display() const override {
        std::cout << "Derived(" << getValue() << ")\n";
    }
};

class FinalDerived final : public MyClass {
public:
    explicit FinalDerived(int v) noexcept : MyClass{v} {}

    void display() const override {
        std::cout << "FinalDerived(" << getValue() << ")\n";
    }
};

class TaggedDerived : public MyClass {
public:
    TaggedDerived(int v, std::string tag) noexcept : MyClass{v}, tag_{std::move(tag)} {}

    void display() const override {
        std::cout << "TaggedDerived(" << getValue() << ", tag=" << tag_ << ")\n";
    }

    [[nodiscard]] bool equals(const MyClass& other) const override {
        if (typeid(*this) != typeid(other))
            return false;
        const auto& rhs = static_cast<const TaggedDerived&>(other);
        return getValue() == rhs.getValue() && tag_ == rhs.tag_;
    }

private:
    std::string tag_;
};

void showObject(const MyClass& obj) {
    std::cout << "[Helper] " << obj << "\n";
}

void printVector(const std::vector<MyClass>& vec) {
    for (const auto& obj : vec) { std::cout << obj << " "; }
    std::cout << "\n";
}

void sortObjects(std::vector<MyClass>& vec) {
    std::ranges::sort(vec, [](const MyClass& lhs, const MyClass& rhs) { return lhs < rhs; });
}

void polymorphic_equality_and_slicing_hazard_demo() {
    std::cout << "\n--- Polymorphic equality hazard (defaulted operator==) ---\n";
    const MyClass base{10};
    const Derived derived{10};
    std::cout << std::boolalpha
              << "base == derived (same value_, different dynamic type): " << (base == derived) << "\n"
              << "(operator==(const MyClass&) silently slices 'derived' to compare only the base part)\n";

    const TaggedDerived tagged_a{10, "left"};
    const TaggedDerived tagged_b{10, "right"};
    std::cout << "tagged_a == tagged_b (same value_, different tag_, naive operator==): "
              << (tagged_a == tagged_b) << "\n"
              << "tagged_a.equals(tagged_b) (virtual equals, dynamic-type + tag aware): "
              << tagged_a.equals(tagged_b) << "\n";

    std::cout << "\n--- Slicing through copy of a base reference ---\n";
    const std::unique_ptr<MyClass> owned = std::make_unique<Derived>(77);
    owned->display();
    const MyClass sliced_copy = *owned;
    std::cout << "sliced_copy.display() after copying through MyClass reference: ";
    sliced_copy.display();
}

void polymorphic_container_demo() {
    std::cout << "\n--- Polymorphic container (vector<unique_ptr<MyClass>>, no slicing) ---\n";
    std::vector<std::unique_ptr<MyClass>> shapes;
    shapes.push_back(std::make_unique<Derived>(1));
    shapes.push_back(std::make_unique<FinalDerived>(2));
    shapes.push_back(std::make_unique<TaggedDerived>(3, "alpha"));
    for (const auto& s : shapes) { s->display(); }
}

int main() {
    MyClass obj{42};
    obj.setValue(100);
    std::cout << "Value: " << obj.getValue() << "\n";
    obj.display();

    std::cout << "\n--- Ref-qualified overloads ---\n";
    obj.process();
    std::move(obj).process();

    const MyClass cobj{300};
    cobj.process();
    std::move(cobj).process();

    std::cout << "\n--- Method chaining ---\n";
    obj.add(10).add(20).multiply(2);
    std::cout << "After chaining: " << obj << "\n";
    obj.safePrint();
    std::cout << "Is positive: " << std::boolalpha << obj.isPositive() << "\n";

    std::cout << "\n--- Const chaining (addConst) ---\n";
    MyClass newObj = cobj.addConst(50);
    std::cout << "New object: " << newObj << "\n";

    std::cout << "\n--- safeGet / classInfo ---\n";
    std::cout << "safeGet: " << obj.safeGet() << "\n";
    MyClass::classInfo();

    std::cout << "\n--- FinalDerived ---\n";
    FinalDerived fd{500};
    fd.display();
    showObject(fd);

    std::cout << "\n--- Virtual dispatch ---\n";
    std::unique_ptr<MyClass> basePtr = std::make_unique<Derived>(200);
    basePtr->display();

    std::cout << "\n--- Operator overloads ---\n";
    MyClass a{10}, b{20}, c{10};
    std::cout << "a == b: " << (a == b) << "\n"
              << "a == c: " << (a == c) << "\n"
              << "a: " << a << "\n";

    std::cout << "\n--- Vector + sort ---\n";
    std::vector<MyClass> vec{MyClass{30}, MyClass{10}, MyClass{20}};
    std::cout << "Before: "; printVector(vec);
    sortObjects(vec);
    std::cout << "After:  "; printVector(vec);

    std::cout << "\n--- Reset ---\n";
    a.reset();
    std::cout << "After reset: " << a << "\n";

    std::cout << "\n--- Smart pointer ---\n";
    auto smartObj = std::make_unique<MyClass>(999);
    assert(smartObj != nullptr);
    smartObj->display();

    polymorphic_equality_and_slicing_hazard_demo();
    polymorphic_container_demo();

    return 0;
}
