// Type Traits Workshop
// Compile-time type introspection

#include <iostream>
#include <type_traits>

template<typename T>
void analyze_type() {
    std::cout << "Type analysis:\n";
    std::cout << "  is_integral: " << std::is_integral_v<T> << "\n";
    std::cout << "  is_pointer: " << std::is_pointer_v<T> << "\n";
    std::cout << "  is_class: " << std::is_class_v<T> << "\n";
    std::cout << "  is_trivial: " << std::is_trivial_v<T> << "\n";
    std::cout << "  size: " << sizeof(T) << "\n";
}

int main() {
    std::cout << "Analyzing int:\n";
    analyze_type<int>();
    
    std::cout << "\nAnalyzing int*:\n";
    analyze_type<int*>();
    
    std::cout << "\nAnalyzing std::string:\n";
    analyze_type<std::string>();
    
    return 0;
}
