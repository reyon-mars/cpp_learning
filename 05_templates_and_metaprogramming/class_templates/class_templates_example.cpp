// Class Templates Exercise
// Generic classes and member templates

#include <iostream>
#include <typeinfo>

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

    return 0;
}
