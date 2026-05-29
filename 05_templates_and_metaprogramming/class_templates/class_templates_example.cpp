#include <iostream>
#include <typeinfo>
#include <utility>   
#include <cassert>   
#include <vector>    
#include <algorithm> 

// Members defined outside the Class definition
template< typename T >
class Stack{
    private:
        T st[MAX];
        int top;
    public:
        Stack();
        void push( T var );
        Type pop();
};

template<class T>
Stack<T>::Stack()
{
    top = -1;
}

template<class T>
void Stack<T>::push( T var )
{
    st[++top] = var;
}

template<class T>
T Stack<T>::pop()
{
    return st[top--];
}

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

    // move constructor
    Container(T&& value)
        : data(std::move(value)) {}

    T get() const { return data; }

    void set(const T& value) {
        data = value;
    }

    // move setter
    void set(T&& value) {
        data = std::move(value);
    }

    template<typename U>
    void convert_from(const U& other) {
        data = static_cast<T>(other);
    }

    // Simple comparison
    bool equals(const T& other) const {
        return data == other;
    }

    // Print type of stored data
    void print_type() const {
        std::cout << "Stored type: "
                  << typeid(T).name() << "\n";
    }

    // Check if default value
    bool is_default() const {
        return data == T{};
    }

    // Swap contents with another container
    void swap(Container<T>& other) {
        using std::swap;
        swap(data, other.data);
    }

    // reset to default
    void reset() {
        data = T{};
    }

    // print stored value
    void print() const {
        std::cout << "Value: " << data << "\n";
    }

    // compare with another container
    bool operator==(const Container<T>& other) const {
        return data == other.data;
    }

    // --------------------------------
};

// Stream output operator
template<typename T>
std::ostream& operator<<(std::ostream& os,
                         const Container<T>& c) {
    return os << c.get();
}


// Generic print helper
template<typename T>
void print_container(const Container<T>& c) {
    std::cout << "[Container] "
              << c << "\n";
}

// Generic max helper
template<typename T>
T max_value(const Container<T>& a,
            const Container<T>& b) {
    return (a.get() > b.get())
               ? a.get()
               : b.get();
}

// --------------------------------

int main() {

    Container<int> int_container(42);

    std::cout << "Int: "
              << int_container << "\n";

    Container<double> double_container(3.14);

    std::cout << "Double: "
              << double_container << "\n";

    double_container.convert_from(100);

    std::cout << "After convert: "
              << double_container << "\n";

    // Cross-type construction
    Container<int> copied_from_double(
        double_container);

    std::cout << "Copied from double: "
              << copied_from_double << "\n";

    // Default constructor + set
    Container<std::string> string_container;

    string_container.set("Templates!");

    std::cout << "String: "
              << string_container << "\n";

    // Comparison demo
    if (int_container.equals(42)) {
        std::cout
            << "int_container equals 42\n";
    }

    // -------- ADDED USAGE --------

    int_container.print_type();
    string_container.print_type();

    std::cout << "Is string_container default? "
              << (string_container.is_default()
                      ? "Yes\n"
                      : "No\n");

    Container<int> another_int(10);

    std::cout << "Before swap: "
              << int_container << ", "
              << another_int << "\n";

    int_container.swap(another_int);

    std::cout << "After swap: "
              << int_container << ", "
              << another_int << "\n";

    assert(int_container.equals(10));

    // -------- NEW FEATURE USAGE --------

    std::cout << "\n--- Reset Demo ---\n";

    another_int.reset();

    std::cout << "After reset: "
              << another_int << "\n";

    std::cout << "Is default? "
              << (another_int.is_default()
                      ? "Yes\n"
                      : "No\n");

    std::cout << "\n--- Print Helper ---\n";

    print_container(int_container);
    print_container(double_container);

    std::cout << "\n--- Max Value ---\n";

    std::cout << "Max: "
              << max_value(int_container,
                           copied_from_double)
              << "\n";

    std::cout << "\n--- Operator== Demo ---\n";

    Container<int> compare_test(10);

    std::cout << "Containers equal? "
              << ((int_container == compare_test)
                      ? "Yes\n"
                      : "No\n");

    std::cout << "\n--- Move Set Demo ---\n";

    Container<std::string> moved_string;

    std::string temp = "Move semantics";

    moved_string.set(std::move(temp));

    moved_string.print();

    std::cout << "Original string after move: "
              << temp << "\n";

    std::cout << "\n--- Vector of Containers ---\n";

    std::vector<Container<int>> vec;

    vec.emplace_back(1);
    vec.emplace_back(2);
    vec.emplace_back(3);

    for (const auto& c : vec) {
        std::cout << c << " ";
    }

    std::cout << "\n";

    std::cout << "\n--- Sorting Containers ---\n";

    std::sort(vec.begin(), vec.end(),
        [](const auto& a, const auto& b) {
            return a.get() > b.get();
        });

    for (const auto& c : vec) {
        std::cout << c << " ";
    }

    std::cout << "\n";

    // ----------------------------

    return 0;
}
