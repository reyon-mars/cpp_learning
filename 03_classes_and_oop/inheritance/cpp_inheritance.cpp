#include <iostream>
#include <memory>
#include <string_view>
#include <cassert>
#include <format>
#include <vector>
#include <functional>

class Super {
public:
    Super() noexcept : accessible_storage_{0}, storage_{0} {}
    explicit Super(int val) noexcept : accessible_storage_{val}, storage_{val} {}

    virtual ~Super() { std::cout << "~Super\n"; }

    void set(int val)   noexcept { storage_ = accessible_storage_ = val; }
    void reset()        noexcept { storage_ = accessible_storage_ = 0; }
    [[nodiscard]] int get() const noexcept { return storage_; }

    virtual void info()                const { std::cout << "Super\n"; }
    [[nodiscard]] virtual std::string_view class_name() const noexcept { return "Super"; }

protected:
    int accessible_storage_;

private:
    int storage_;
};

class Base {
public:
    Base() noexcept : storage_{0} {}
    explicit Base(int val) noexcept : storage_{val} {}

    virtual ~Base() { std::cout << "~Base\n"; }

    void set(int val) noexcept { storage_ = val; }
    void reset()      noexcept { storage_ = 0; }
    [[nodiscard]] int get() const noexcept { return storage_; }

    virtual void info()                const { std::cout << "Base\n"; }
    [[nodiscard]] virtual std::string_view class_name() const noexcept { return "Base"; }

protected:
    int storage_;
};

class Sub final : public Super {
public:
    void print() const {
        std::cout << std::format("Sub storage = {}\n", accessible_storage_);
    }

    void increment()  noexcept { ++accessible_storage_; }
    void decrement()  noexcept { --accessible_storage_; }

    void info() const override { std::cout << "Sub (derives Super)\n"; }
    [[nodiscard]] std::string_view class_name() const noexcept override { return "Sub"; }

    void show_address() const {
        std::cout << std::format("Sub address: {}\n", static_cast<const void*>(this));
    }
};

class SubMulti final : public Super, public Base {
public:
    SubMulti() noexcept = default;
    SubMulti(int superVal, int baseVal) noexcept
        : Super{superVal}, Base{baseVal} {}

    void print() const {
        std::cout << std::format("Super storage={} Base storage={}\n",
            Super::accessible_storage_, Base::storage_);
    }

    [[nodiscard]] int total_storage() const noexcept {
        return Super::accessible_storage_ + Base::storage_;
    }

    void set_both(int a, int b) noexcept { Super::set(a); Base::set(b); }
    void reset_both()           noexcept { Super::reset(); Base::reset(); }
    void increment_both()       noexcept { ++Super::accessible_storage_; ++Base::storage_; }

    void show_info() const { Super::info(); Base::info(); }

    void print_addresses() const {
        std::cout << std::format("Super part: {}\nBase part:  {}\n",
            static_cast<const void*>(static_cast<const Super*>(this)),
            static_cast<const void*>(static_cast<const Base*>(this)));
    }

    void info() const override { std::cout << "SubMulti (multiple inheritance)\n"; }
    [[nodiscard]] std::string_view class_name() const noexcept override { return "SubMulti"; }
};

class SubExtended : public Super {
    std::string tag_;
public:
    explicit SubExtended(std::string tag, int val = 0)
        : Super{val}, tag_(std::move(tag)) {}

    void info() const override {
        std::cout << std::format("SubExtended[{}] value={}\n", tag_, get());
    }

    [[nodiscard]] std::string_view class_name() const noexcept override { return "SubExtended"; }
    [[nodiscard]] std::string_view tag()         const noexcept          { return tag_;          }
};

void polymorphism_demo(Super& obj) {
    std::cout << "Virtual dispatch: ";
    obj.info();
}

void type_check(Super& obj) {
    std::cout << "Type check: ";
    if (dynamic_cast<Sub*>(&obj))           std::cout << "Sub\n";
    else if (dynamic_cast<SubMulti*>(&obj)) std::cout << "SubMulti\n";
    else if (dynamic_cast<SubExtended*>(&obj)) std::cout << "SubExtended\n";
    else                                    std::cout << "other Super-derived\n";
}

void smart_pointer_demo() {
    auto ptr = std::make_unique<Sub>();
    std::cout << std::format("unique_ptr: class={}\n", ptr->class_name());
    ptr->info();
}

void virtual_dispatch_via_vector() {
    std::vector<std::unique_ptr<Super>> objects;
    objects.push_back(std::make_unique<Sub>());
    objects.push_back(std::make_unique<SubExtended>("alpha", 10));
    objects.push_back(std::make_unique<SubExtended>("beta",  20));

    std::cout << "\n--- Vector polymorphism ---\n";
    for (const auto& obj : objects) {
        std::cout << std::format("  class={} | ", obj->class_name());
        obj->info();
    }
}

void slice_demo() {
    Sub s;
    s.set(99);
    Super sliced = s;
    std::cout << std::format("Original Sub class_name={}\n", s.class_name());
    std::cout << std::format("Sliced Super class_name={} value={}\n",
        sliced.class_name(), sliced.get());
    std::cout << std::format("Object slicing confirmed: type changed={}\n",
        std::string_view{sliced.class_name()} != std::string_view{s.class_name()});
}

int main() {
    SubMulti object;
    object.Super::set(100);
    object.Base::set(50);
    object.Super::set(object.Super::get() + 1);
    object.print();
    std::cout << std::format("Total storage = {}\n", object.total_storage());

    object.reset_both();
    std::cout << "\nAfter reset:\n";
    object.print();

    std::cout << "\n--- set_both ---\n";
    SubMulti obj2{10, 20};
    obj2.print();
    obj2.set_both(30, 40);
    std::cout << "After set_both:\n";
    obj2.print();
    std::cout << std::format("Super::get={} Base::get={}\n",
        obj2.Super::get(), obj2.Base::get());
    obj2.show_info();

    std::cout << "\n--- increment_both ---\n";
    obj2.increment_both();
    obj2.print();
    obj2.print_addresses();

    std::cout << "\n--- Polymorphism ---\n";
    Sub s;
    polymorphism_demo(s);

    std::cout << "\n--- Type check ---\n";
    type_check(s);
    type_check(obj2);

    std::cout << "\n--- class_name via virtual ---\n";
    std::cout << std::format("s class_name={}\n",    s.class_name());
    std::cout << std::format("obj2 class_name={}\n", obj2.class_name());

    std::cout << "\n--- Smart pointer ---\n";
    smart_pointer_demo();

    std::cout << "\n--- Sub features ---\n";
    s.increment();
    s.print();
    s.decrement();
    s.print();
    s.show_address();

    std::cout << "\n--- SubExtended ---\n";
    SubExtended se("gamma", 42);
    se.info();
    std::cout << std::format("tag={} get={}\n", se.tag(), se.get());
    polymorphism_demo(se);
    type_check(se);

    virtual_dispatch_via_vector();

    std::cout << "\n--- Object slicing demo ---\n";
    slice_demo();

    assert(dynamic_cast<Sub*>(&s)                   != nullptr);
    assert(dynamic_cast<SubMulti*>(&obj2)           != nullptr);
    assert(dynamic_cast<SubExtended*>(&se)          != nullptr);
    assert(dynamic_cast<Sub*>(static_cast<Super*>(&obj2)) == nullptr);

    std::cout << "\nAll assertions passed.\n";
    return 0;
}
