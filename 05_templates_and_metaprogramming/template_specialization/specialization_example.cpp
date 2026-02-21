// Template Specialization Exercise
// Full and partial specialization

#include <iostream>
#include <vector>

// Primary template
template<typename T>
struct traits {
    static void print() { std::cout << "Generic template\n"; }
};

// Full specialization
template<>
struct traits<int> {
    static void print() { std::cout << "Specialized for int\n"; }
};

// Partial specialization
template<typename T>
struct traits<T*> {
    static void print() { std::cout << "Specialized for pointers\n"; }
};

// Partial specialization for vectors
template<typename T>
struct traits<std::vector<T>> {
    static void print() { std::cout << "Specialized for vector\n"; }
};

int main() {
    traits<double>::print();
    traits<int>::print();
    traits<double*>::print();
    traits<std::vector<int>>::print();
    
    return 0;
}
