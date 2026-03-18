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

        // ----------- NEW ADDITIONS -----------
        
        // Constructor
        MyClass() : value(0) {
            std::cout << "MyClass constructed\n";
        }

        // Setter
        void setValue(int v) {
            value = v;
        }

        // Getter
        int getValue() const {
            return value;
        }

        // Static utility function
        static void staticInfo() {
            std::cout << "Static function inside MyClass\n";
        }

        // ------------------------------------

    private:
        int value;  // internal state
    };

    // ---- small helper function ----
    void print_info(const MyClass& obj) {
        std::cout << "Object name: " << obj.name() << "\n";
    }
    // -------------------------------

    // ----------- NEW ADDITIONS -----------

    // Another helper function
    void print_value(const MyClass& obj) {
        std::cout << "Stored value: " << obj.getValue() << "\n";
    }

    // Factory function
    MyClass create_object(int v) {
        MyClass temp;
        temp.setValue(v);
        return temp;
    }

    // ------------------------------------

} // namespace my_lib

int main() {

    // ---------- ORIGINAL CODE ----------
    my_lib::MyClass obj;
    obj.print();
    // -----------------------------------

    // ---- small added usage ----
    my_lib::print_info(obj);
    // ---------------------------

    // -------- NEW FEATURE USAGE --------
    obj.setValue(42);
    my_lib::print_value(obj);

    my_lib::MyClass obj2 = my_lib::create_object(100);
    my_lib::print_value(obj2);

    my_lib::MyClass::staticInfo();
    // ----------------------------------

    return 0;
}
