// Function Templates Exercise
// Template specialization and instantiation

#include <iostream>
#include <string>

// Generic template
template<typename T>
void print(const T& value) {
    std::cout << "Value: " << value << "\n";
}

// Specialization for std::string
template<>
void print<std::string>(const std::string& value) {
    std::cout << "String: [" << value << "]\n";
}

// Overload
template<typename T, typename U>
void print(const T& a, const U& b) {
    std::cout << "Two values: " << a << ", " << b << "\n";
}

int main() {
    print(42);
    print(3.14);
    print(std::string("Hello"));
    print(10, "test");
    
    return 0;
}
