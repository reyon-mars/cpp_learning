#include <iostream>
#include <memory>
#include <utility>
#include <cassert>
#include <vector>
#include <string_view>

class Widget {
public:
    Widget()                      { std::cout << "Widget created\n";   }
    Widget(const Widget&)         { std::cout << "Widget copied\n";    }
    Widget(Widget&&) noexcept     { std::cout << "Widget moved\n";     }
    ~Widget()                     { std::cout << "Widget destroyed\n"; }

    Widget& operator=(const Widget&)     = default;
    Widget& operator=(Widget&&) noexcept = default;

    void hello() const { std::cout << "Hello from Widget\n"; }
};

struct NodeBad {
    std::shared_ptr<NodeBad> next;
    ~NodeBad() { std::cout << "NodeBad destroyed\n"; }
};

struct NodeGood {
    std::weak_ptr<NodeGood> next;
    ~NodeGood() { std::cout << "NodeGood destroyed\n"; }
};

class SelfShared : public std::enable_shared_from_this<SelfShared> {
public:
    SelfShared()  { std::cout << "SelfShared created\n";   }
    ~SelfShared() { std::cout << "SelfShared destroyed\n"; }

    [[nodiscard]] std::shared_ptr<SelfShared> getShared() {
        return shared_from_this();
    }
};

void print_use_count(const std::shared_ptr<Widget>& ptr, std::string_view name) {
    std::cout << name << " use_count=" << ptr.use_count() << "\n";
}

int main() {
    std::cout << "=== unique_ptr ===\n";
    {
        auto ptr1 = std::make_unique<Widget>();
        auto ptr2 = std::move(ptr1);
        assert(ptr1 == nullptr);

        Widget* raw = ptr2.release();
        std::cout << "Released\n";
        delete raw;

        ptr2 = std::make_unique<Widget>();
        ptr2.reset();
        assert(ptr2 == nullptr);

        ptr2 = std::make_unique<Widget>();
        std::cout << "Raw address=" << static_cast<void*>(ptr2.get()) << "\n";
        ptr2->hello();

        auto ptr3 = std::make_unique<Widget>();
        ptr2.swap(ptr3);
        std::cout << "Swapped\n";
        assert(ptr2 != nullptr && ptr3 != nullptr);

        std::vector<std::unique_ptr<Widget>> widgets;
        widgets.reserve(2);
        widgets.push_back(std::make_unique<Widget>());
        widgets.push_back(std::make_unique<Widget>());
        std::cout << "Vector size=" << widgets.size() << "\n";
    }

    std::cout << "\n=== shared_ptr ===\n";
    {
        auto ptr1 = std::make_shared<Widget>();
        print_use_count(ptr1, "ptr1");

        auto ptr2 = ptr1;
        print_use_count(ptr1, "ptr1 after copy");

        {
            auto ptr3 = ptr2;
            print_use_count(ptr1, "ptr1 with ptr3 alive");
        }
        print_use_count(ptr1, "ptr1 after ptr3 gone");

        ptr2.reset();
        print_use_count(ptr1, "ptr1 after ptr2.reset");

        auto ptr_new = std::make_shared<Widget>();
        print_use_count(ptr_new, "ptr_new");
        print_use_count(ptr1, "ptr1 (exclusive)");
    }

    std::cout << "\n=== weak_ptr ===\n";
    {
        auto sp = std::make_shared<Widget>();
        std::weak_ptr<Widget> wp = sp;
        std::cout << "use_count=" << sp.use_count() << "\n";

        if (auto locked = wp.lock()) {
            std::cout << "Locked: ";
            locked->hello();
        }

        sp.reset();
        std::cout << "expired=" << std::boolalpha << wp.expired() << "\n"
                  << "use_count via weak=" << wp.use_count() << "\n";
    }

    std::cout << "\n=== Custom deleter ===\n";
    {
        auto deleter = [](Widget* w) {
            std::cout << "Custom deleter\n";
            delete w;
        };
        std::unique_ptr<Widget, decltype(deleter)> ptr{new Widget, deleter};
    }

    std::cout << "\n=== Circular reference (leak) ===\n";
    {
        auto n1 = std::make_shared<NodeBad>();
        auto n2 = std::make_shared<NodeBad>();
        n1->next = n2;
        n2->next = n1;
        std::cout << "NodeBad cycle created — destructors will NOT run\n";
    }

    std::cout << "\n=== Circular reference fixed (weak_ptr) ===\n";
    {
        auto n1 = std::make_shared<NodeGood>();
        auto n2 = std::make_shared<NodeGood>();
        n1->next = n2;
        n2->next = n1;
        std::cout << "NodeGood cycle — destructors WILL run\n";
    }

    std::cout << "\n=== Aliasing constructor ===\n";
    {
        auto sp    = std::make_shared<Widget>();
        auto alias = std::shared_ptr<Widget>{sp, sp.get()};
        std::cout << "Aliasing use_count=" << sp.use_count() << "\n";
    }

    std::cout << "\n=== make_unique vs raw new ===\n";
    {
        auto p1 = std::make_unique<Widget>();
        auto p2 = std::unique_ptr<Widget>{new Widget};
        std::cout << "Prefer make_unique\n";
    }

    std::cout << "\n=== shared_ptr reset with new object ===\n";
    {
        auto sp = std::make_shared<Widget>();
        sp.reset(new Widget);
        std::cout << "Reset with new object\n";
    }

    std::cout << "\n=== enable_shared_from_this ===\n";
    {
        auto self  = std::make_shared<SelfShared>();
        auto self2 = self->getShared();
        std::cout << "use_count=" << self.use_count() << "\n";
    }

    std::cout << "\n=== weak_ptr observer ===\n";
    {
        std::weak_ptr<Widget> observer;
        {
            auto owner  = std::make_shared<Widget>();
            observer    = owner;
            std::cout << "Observer use_count=" << observer.use_count() << "\n";
        }
        std::cout << "Observer expired=" << observer.expired() << "\n";
    }

    return 0;
}
