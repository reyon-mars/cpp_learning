#include <iostream>
#include <memory>
#include <utility>
#include <cassert>
#include <vector>
#include <string_view>
#include <format>
#include <span>
#include <functional>
#include <array>

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

    [[nodiscard]] std::shared_ptr<SelfShared> get_shared() {
        return shared_from_this();
    }

    [[nodiscard]] std::weak_ptr<SelfShared> get_weak() {
        return weak_from_this();
    }
};

struct FileDeleter {
    void operator()(std::FILE* f) const noexcept {
        if (f) {
            std::cout << "FileDeleter: closing file\n";
            std::fclose(f);
        }
    }
};

using FilePtr = std::unique_ptr<std::FILE, FileDeleter>;

[[nodiscard]] FilePtr open_file(const char* name, const char* mode) {
    return FilePtr{std::fopen(name, mode)};
}

[[nodiscard]] std::unique_ptr<Widget> make_widget() {
    return std::make_unique<Widget>();
}

[[nodiscard]] std::shared_ptr<Widget> make_shared_widget() {
    return std::make_shared<Widget>();
}

void print_use_count(const std::shared_ptr<Widget>& ptr, std::string_view name) {
    std::cout << std::format("{} use_count={}\n", name, ptr.use_count());
}

void take_unique(std::unique_ptr<Widget> w) {
    std::cout << "Taking ownership: ";
    w->hello();
}

void borrow(const Widget& w) {
    std::cout << "Borrowing: ";
    w.hello();
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
        std::cout << std::format("Raw address={}\n", static_cast<void*>(ptr2.get()));
        ptr2->hello();

        auto ptr3 = std::make_unique<Widget>();
        ptr2.swap(ptr3);
        std::cout << "Swapped\n";
        assert(ptr2 != nullptr && ptr3 != nullptr);

        std::vector<std::unique_ptr<Widget>> widgets;
        widgets.reserve(2);
        widgets.push_back(std::make_unique<Widget>());
        widgets.push_back(std::make_unique<Widget>());
        std::cout << std::format("Vector size={}\n", widgets.size());
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
        print_use_count(ptr1,    "ptr1 (exclusive)");
    }

    std::cout << "\n=== weak_ptr ===\n";
    {
        auto sp = std::make_shared<Widget>();
        std::weak_ptr<Widget> wp = sp;
        std::cout << std::format("use_count={}\n", sp.use_count());

        if (auto locked = wp.lock()) {
            std::cout << "Locked: ";
            locked->hello();
        }

        sp.reset();
        std::cout << std::format("expired={}\n", wp.expired());
        std::cout << std::format("use_count via weak={}\n", wp.use_count());
    }

    std::cout << "\n=== Custom deleter (lambda) ===\n";
    {
        auto deleter = [](Widget* w) {
            std::cout << "Custom deleter\n";
            delete w;
        };
        std::unique_ptr<Widget, decltype(deleter)> ptr{new Widget, deleter};
    }

    std::cout << "\n=== Custom deleter (struct: FILE*) ===\n";
    {
        auto fp = open_file("/dev/null", "r");
        if (fp) std::cout << "File opened via FilePtr\n";
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
        std::cout << std::format("Aliasing use_count={}\n", sp.use_count());
    }

    std::cout << "\n=== make_unique vs raw new ===\n";
    {
        auto p1 = make_widget();
        auto p2 = std::unique_ptr<Widget>{new Widget};
        std::cout << "Prefer make_unique (exception safety + clarity)\n";
    }

    std::cout << "\n=== shared_ptr reset with new object ===\n";
    {
        auto sp = std::make_shared<Widget>();
        sp.reset(new Widget);
        std::cout << "Reset with new object\n";
    }

    std::cout << "\n=== enable_shared_from_this ===\n";
    {
        auto self   = std::make_shared<SelfShared>();
        auto self2  = self->get_shared();
        std::cout << std::format("use_count={}\n", self.use_count());

        std::weak_ptr<SelfShared> weak = self->get_weak();
        std::cout << std::format("weak expired (while alive)={}\n", weak.expired());
    }

    std::cout << "\n=== weak_ptr observer ===\n";
    {
        std::weak_ptr<Widget> observer;
        {
            auto owner = std::make_shared<Widget>();
            observer   = owner;
            std::cout << std::format("Observer use_count={}\n", observer.use_count());
        }
        std::cout << std::format("Observer expired={}\n", observer.expired());
    }

    std::cout << "\n=== shared_ptr to array (C++17) ===\n";
    {
        auto arr = std::make_shared<Widget[]>(3);
        std::cout << std::format("shared_ptr<Widget[]> size via raw pointer\n");
        std::span<Widget> view{arr.get(), 3};
        for (const auto& w : view) w.hello();
    }

    std::cout << "\n=== unique_ptr ownership transfer into function ===\n";
    {
        auto w = std::make_unique<Widget>();
        borrow(*w);
        take_unique(std::move(w));
        assert(w == nullptr);
    }

    std::cout << "\n=== shared_ptr from factory ===\n";
    {
        auto sp = make_shared_widget();
        std::cout << std::format("Factory shared use_count={}\n", sp.use_count());
        auto sp2 = sp;
        std::cout << std::format("After copy use_count={}\n", sp.use_count());
        sp2.reset();
        std::cout << std::format("After sp2.reset use_count={}\n", sp.use_count());
    }

    std::cout << "\n=== std::function as deleter (type-erased) ===\n";
    {
        std::function<void(Widget*)> dyn_deleter = [](Widget* w) {
            std::cout << "Dynamic function deleter\n";
            delete w;
        };
        std::shared_ptr<Widget> sp{new Widget, dyn_deleter};
        sp->hello();
    }

    return 0;
}
