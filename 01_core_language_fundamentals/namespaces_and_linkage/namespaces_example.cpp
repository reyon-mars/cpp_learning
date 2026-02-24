#include <iostream>

namespace my_lib {

    class MyClass {
    public:
        void print() const {
            std::cout << "MyClass\n";
        }

        // ---- small addition ----
        const char* name() const {
            return "MyClass";
        }
        // ------------------------
    };

    // ---- small helper function ----
    void print_info(const MyClass& obj) {
        std::cout << "Object name: " << obj.name() << "\n";
    }
    // -------------------------------

} // namespace my_lib

int main() {

    // ---------- ORIGINAL CODE ----------
    my_lib::MyClass obj;
    obj.print();
    // -----------------------------------

    // ---- small added usage ----
    my_lib::print_info(obj);
    // ---------------------------

    return 0;
}
