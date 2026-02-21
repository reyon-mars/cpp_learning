// Namespaces and Linkage Exercise
// Internal vs external linkage, namespace scoping

#include <iostream>

namespace my_lib {
    class MyClass {
    public:
        void print() { std::cout << "MyClass\n"; }
    };
}

int main() {
    my_lib::MyClass obj;
    obj.print();
    return 0;
}
