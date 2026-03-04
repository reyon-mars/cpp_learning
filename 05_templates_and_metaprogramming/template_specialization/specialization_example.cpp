// Template Specialization Exercise
// Full and partial specialization

#include <iostream>
#include <vector>

// ----------------------------------
// Primary template
// ----------------------------------
template<typename T>
struct traits {
    static constexpr const char* name = "Generic";
    static void print() { 
        std::cout << "Generic template\n"; 
    }
};

// ----------------------------------
// Full specialization
// ----------------------------------
template<>
struct traits<int> {
    static constexpr const char* name = "int";
    static void print() { 
        std::cout << "Specialized for int\n"; 
    }
};

// ----------------------------------
// Partial specialization (pointers)
// ----------------------------------
template<typename T>
struct traits<T*> {
    static constexpr const char* name = "pointer";
    static void print() { 
        std::cout << "Specialized for pointers\n"; 
    }
};

// ----------------------------------
// Partial specialization (const types)
// ----------------------------------
template<typename T>
struct traits<const T> {
    static constexpr const char* name = "const type";
    static void print() { 
        std::cout << "Specialized for const types\n"; 
    }
};

// ----------------------------------
// Partial specialization (vector)
// ----------------------------------
template<typename T>
struct traits<std::vector<T>> {
    static constexpr const char* name = "vector";
    static void print() { 
        std::cout << "Specialized for vector\n"; 
    }
};

// More specific specialization
template<>
struct traits<std::vector<int>> {
    static constexpr const char* name = "vector<int>";
    static void print() { 
        std::cout << "Specialized specifically for vector<int>\n"; 
    }
};

// ----------------------------------
// Main
// ----------------------------------
int main() {

    traits<double>::print();
    traits<int>::print();
    traits<double*>::print();
    traits<const float>::print();
    traits<std::vector<double>>::print();
    traits<std::vector<int>>::print();

    return 0;
}
