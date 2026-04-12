#include <iostream>
#include <memory>

class Widget {
public:
    Widget() { std::cout << "Widget created\n"; }
    ~Widget() { std::cout << "Widget destroyed\n"; }
};

// -------- NEW ADDITIONS --------

// Circular reference demo (problem)
struct NodeBad {
    std::shared_ptr<NodeBad> next;
    ~NodeBad() { std::cout << "NodeBad destroyed\n"; }
};

// Fixed using weak_ptr
struct NodeGood {
    std::weak_ptr<NodeGood> next;
    ~NodeGood() { std::cout << "NodeGood destroyed\n"; }
};

// --------------------------------

int main() {

    // unique_ptr: exclusive ownership
    {
        std::unique_ptr<Widget> ptr1 = std::make_unique<Widget>();

        std::unique_ptr<Widget> ptr2 = std::move(ptr1);

        // release demo
        Widget* raw = ptr2.release();
        std::cout << "Released ownership\n";
        delete raw;

        // reset demo
        ptr2 = std::make_unique<Widget>();
        ptr2.reset();

        // -------- NEW USAGE --------
        ptr2 = std::make_unique<Widget>();
        std::cout << "Raw pointer access: " << ptr2.get() << "\n";

        // EXTRA: swap demo
        std::unique_ptr<Widget> ptr3 = std::make_unique<Widget>();
        ptr2.swap(ptr3);
        std::cout << "Swapped unique_ptrs\n";
        // --------------------------------
    }

    std::cout << "----------------------\n";

    // shared_ptr: shared ownership
    {
        std::shared_ptr<Widget> ptr1 = std::make_shared<Widget>();
        std::cout << "Ref count: " << ptr1.use_count() << "\n";

        std::shared_ptr<Widget> ptr2 = ptr1;
        std::cout << "Ref count after copy: " << ptr1.use_count() << "\n";

        {
            std::shared_ptr<Widget> ptr3 = ptr2;
            std::cout << "Ref count after another copy: "
                      << ptr1.use_count() << "\n";
        }

        std::cout << "Ref count after inner scope: "
                  << ptr1.use_count() << "\n";

        ptr2.reset();
        std::cout << "Ref count after reset: "
                  << ptr1.use_count() << "\n";

        // -------- NEW USAGE --------
        std::shared_ptr<Widget> ptr_new(new Widget());
        std::cout << "Using new (not recommended) count: "
                  << ptr_new.use_count() << "\n";

        // EXTRA: unique() check
        std::cout << "Is unique? " << ptr1.unique() << "\n";
        // --------------------------------
    }

    std::cout << "----------------------\n";

    // weak_ptr: non-owning reference
    {
        std::shared_ptr<Widget> sp = std::make_shared<Widget>();
        std::weak_ptr<Widget> wp = sp;

        std::cout << "Shared count: " << sp.use_count() << "\n";

        if (auto locked = wp.lock()) {
            std::cout << "Weak_ptr locked successfully\n";
        }

        sp.reset();

        if (wp.expired()) {
            std::cout << "Weak_ptr expired\n";
        }

        // EXTRA: use_count via weak_ptr
        std::cout << "Weak_ptr use_count: "
                  << wp.use_count() << "\n";
    }

    std::cout << "----------------------\n";

    // Custom deleter example
    {
        std::unique_ptr<Widget, void(*)(Widget*)> ptr(
            new Widget(),
            [](Widget* w) {
                std::cout << "Custom deleting Widget\n";
                delete w;
            }
        );
    }

    std::cout << "----------------------\n";

    // -------- NEW FEATURE: Circular reference problem --------
    {
        auto n1 = std::make_shared<NodeBad>();
        auto n2 = std::make_shared<NodeBad>();

        n1->next = n2;
        n2->next = n1;

        std::cout << "NodeBad circular reference created\n";
    } // ❌ memory leak (destructors NOT called)

    std::cout << "----------------------\n";

    // -------- FIX using weak_ptr --------
    {
        auto n1 = std::make_shared<NodeGood>();
        auto n2 = std::make_shared<NodeGood>();

        n1->next = n2;
        n2->next = n1;

        std::cout << "NodeGood with weak_ptr\n";
    } // ✅ properly destroyed

    std::cout << "----------------------\n";

    // -------- ADVANCED: aliasing constructor --------
    {
        auto sp = std::make_shared<Widget>();
        std::shared_ptr<Widget> alias(sp, sp.get());

        std::cout << "Aliasing shared_ptr count: "
                  << sp.use_count() << "\n";
    }

    // -------- EXTRA SMALL ADDITIONS --------

    // make_unique vs new comparison
    {
        auto p1 = std::make_unique<Widget>();
        auto p2 = std::unique_ptr<Widget>(new Widget());

        std::cout << "Both created (prefer make_unique)\n";
    }

    // shared_ptr reset with new object
    {
        auto sp = std::make_shared<Widget>();
        sp.reset(new Widget());
        std::cout << "shared_ptr reset with new object\n";
    }

    // --------------------------------------

    return 0;
}
