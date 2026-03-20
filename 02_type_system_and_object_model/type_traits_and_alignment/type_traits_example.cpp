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

// ----------- NEW ADDITIONS -----------

// Extended trait printer
template<typename T>
void print_more_traits() {
    std::cout << "Is const: " << std::is_const<T>::value << "\n";
    std::cout << "Is reference: " << std::is_reference<T>::value << "\n";
    std::cout << "Is same as int: "
              << std::is_same<T, int>::value << "\n";
}

// Compile-time behavior
template<typename T>
void analyze_type() {
    if constexpr (std::is_integral<T>::value) {
        std::cout << "This is an integral type\n";
    } else {
        std::cout << "This is NOT an integral type\n";
    }
}

// Custom aligned struct
struct alignas(32) AlignedStruct {
    int x;
    double y;
};

// ------------------------------------

int main() {
    print_traits<int>();
    print_traits<int*>();
    
    alignas(64) char buffer[256];
    std::cout << "Buffer alignment: " << alignof(decltype(buffer)) << "\n";


    // -------- NEW FEATURE USAGE --------

    std::cout << "\nMore traits for int:\n";
    print_more_traits<int>();

    std::cout << "\nMore traits for const int&:\n";
    print_more_traits<const int&>();

    std::cout << "\nAnalyze types:\n";
    analyze_type<int>();
    analyze_type<double>();

    std::cout << "\nCustom aligned struct:\n";
    std::cout << "Size: " << sizeof(AlignedStruct) << "\n";
    std::cout << "Alignment: " << alignof(AlignedStruct) << "\n";

    std::cout << "alignment_of<int>: "
              << std::alignment_of<int>::value << "\n";

    // ----------------------------------

    return 0;
}
