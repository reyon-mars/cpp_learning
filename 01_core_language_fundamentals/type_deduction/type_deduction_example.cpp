// Type Deduction Exercise
// auto, decltype, and deduction rules

#include <iostream>
#include <type_traits>

template<typename T>
void print_type() {
    std::cout << "Type: " << typeid(T).name() << "\n";
}

int main() {
    auto x = 10;           // int
    auto y = 3.14;         // double
    
    int arr[] = {1, 2, 3};
    auto z = arr;          // int*
    
    int a = 5;
    decltype(a) b = 10;    // int
    
    return 0;
}
