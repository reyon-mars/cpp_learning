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
    }  // Automatic cleanup
    
    // shared_ptr: shared ownership
    {
        std::shared_ptr<Widget> ptr1 = std::make_shared<Widget>();
        std::shared_ptr<Widget> ptr2 = ptr1;  // OK: copy
        std::cout << "Ref count: " << ptr1.use_count() << "\n";
    }  // Cleanup when last ptr is destroyed
    
    return 0;
}
