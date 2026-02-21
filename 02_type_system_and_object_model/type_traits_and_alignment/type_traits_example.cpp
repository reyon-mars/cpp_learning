// Type Traits and Alignment Exercise
// std::is_integral, std::is_pointer, alignof, alignas

#include <iostream>
#include <type_traits>

template<typename T>
void print_traits() {
    std::cout << "Is integral: " << std::is_integral<T>::value << "\n";
    std::cout << "Is pointer: " << std::is_pointer<T>::value << "\n";
    std::cout << "Size: " << sizeof(T) << "\n";
    std::cout << "Alignment: " << alignof(T) << "\n";
}

int main() {
    print_traits<int>();
    print_traits<int*>();
    
    alignas(64) char buffer[256];
    std::cout << "Buffer alignment: " << alignof(decltype(buffer)) << "\n";
    
    return 0;
}
