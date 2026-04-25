#include <iostream>
#include <typeinfo>
#include <utility>   // ✅ ADDED
#include <cassert>   // ✅ ADDED

template<typename T>
class Container {
private:
    T data;

public:
    // Default constructor
    Container() : data{} {}

    Container(const T& value) : data(value) {}

    // Templated copy constructor (cross-type)
    template<typename U>
    Container(const Container<U>& other)
        : data(static_cast<T>(other.get())) {}

    T get() const { return data; }
    void set(const T& value) { data = value; }

    template<typename U>
    void convert_from(const U& other) {
        data = static_cast<T>(other);
    }

    // Simple comparison
    bool equals(const T& other) const {
        return data == other;
    }

    // -------- SMALL ADDITIONS --------

    // Print type of stored data
    void print_type() const {
        std::cout << "Stored type: " << typeid(T).name() << "\n";
    }

    // Check if default value
    bool is_default() const {
        return data == T{};
    }

    // Swap contents with another container
    void swap(Container<T>& other) {
        using std::swap;   // ✅ ADDED (best practice)
        swap(data, other.data);
    }

    // --------------------------------
};

// Stream output operator
template<typename T>
std::ostream& operator<<(std::ostream& os, const Container<T>& c) {
    return os << c.get();
}

int main() {

    Container<int> int_container(42);
    std::cout << "Int: " << int_container << "\n";

    Container<double> double_container(3.14);
    std::cout << "Double: " << double_container << "\n";

    double_container.convert_from(100);
    std::cout << "After convert: " << double_container << "\n";

    // Cross-type construction
    Container<int> copied_from_double(double_container);
    std::cout << "Copied from double: "
              << copied_from_double << "\n";

    // Default constructor + set
    Container<std::string> string_container;
    string_container.set("Templates!");
    std::cout << "String: " << string_container << "\n";

    // Comparison demo
    if (int_container.equals(42)) {
        std::cout << "int_container equals 42\n";
    }

    // -------- ADDED USAGE --------

    int_container.print_type();
    string_container.print_type();

    std::cout << "Is string_container default? "
              << (string_container.is_default() ? "Yes\n" : "No\n");

    Container<int> another_int(10);
    std::cout << "Before swap: "
              << int_container << ", " << another_int << "\n";

    int_container.swap(another_int);

    std::cout << "After swap: "
              << int_container << ", " << another_int << "\n";

    // ✅ ADDED: small validation
    assert(int_container.equals(10));

    // ----------------------------

    return 0;
}
