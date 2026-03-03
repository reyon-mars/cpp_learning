// Smart Pointers Workshop
// unique_ptr, shared_ptr, weak_ptr

#include <iostream>
#include <memory>

class Widget {
public:
    Widget() { std::cout << "Widget created\n"; }
    ~Widget() { std::cout << "Widget destroyed\n"; }
};

int main() {

    // unique_ptr: exclusive ownership
    {
        std::unique_ptr<Widget> ptr1 = std::make_unique<Widget>();

        // std::unique_ptr<Widget> ptr2 = ptr1;  // Error: cannot copy
        std::unique_ptr<Widget> ptr2 = std::move(ptr1);  // OK: move

        // release demo
        Widget* raw = ptr2.release();  // ownership released
        std::cout << "Released ownership\n";
        delete raw;  // manual delete required

        // reset demo
        ptr2 = std::make_unique<Widget>();
        ptr2.reset();  // destroys managed object
    }  // Automatic cleanup (if owning something)

    std::cout << "----------------------\n";

    // shared_ptr: shared ownership
    {
        std::shared_ptr<Widget> ptr1 = std::make_shared<Widget>();
        std::cout << "Ref count: " << ptr1.use_count() << "\n";

        std::shared_ptr<Widget> ptr2 = ptr1;  // shared ownership
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
    }  // destroyed when last shared_ptr goes out of scope

    std::cout << "----------------------\n";

    // weak_ptr: non-owning reference
    {
        std::shared_ptr<Widget> sp = std::make_shared<Widget>();
        std::weak_ptr<Widget> wp = sp;

        std::cout << "Shared count: " << sp.use_count() << "\n";

        if (auto locked = wp.lock()) {
            std::cout << "Weak_ptr locked successfully\n";
        }

        sp.reset();  // destroy object

        if (wp.expired()) {
            std::cout << "Weak_ptr expired\n";
        }
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

    return 0;
}
