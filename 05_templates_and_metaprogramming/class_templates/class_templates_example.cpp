// Class Templates Exercise
// Generic classes and member templates

#include <iostream>

template<typename T>
class Container {
private:
    T data;
    
public:
    Container(const T& value) : data(value) {}
    
    T get() const { return data; }
    void set(const T& value) { data = value; }
    
    template<typename U>
    void convert_from(const U& other) {
        data = static_cast<T>(other);
    }
};

int main() {
    Container<int> int_container(42);
    std::cout << "Int: " << int_container.get() << "\n";
    
    Container<double> double_container(3.14);
    std::cout << "Double: " << double_container.get() << "\n";
    
    double_container.convert_from(100);
    std::cout << "After convert: " << double_container.get() << "\n";
    
    return 0;
}
