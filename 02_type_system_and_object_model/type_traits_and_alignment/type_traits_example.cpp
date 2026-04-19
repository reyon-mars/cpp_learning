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

template<typename T>
void print_more_traits() {
    std::cout << "Is const: " << std::is_const<T>::value << "\n";
    std::cout << "Is reference: " << std::is_reference<T>::value << "\n";
    std::cout << "Is same as int: "
              << std::is_same<T, int>::value << "\n";
}

template<typename T>
void analyze_type() {
    if constexpr (std::is_integral<T>::value) {
        std::cout << "This is an integral type\n";
    } else {
        std::cout << "This is NOT an integral type\n";
    }
}

struct alignas(32) AlignedStruct {
    int x;
    double y;
};

// ----------- EXTRA ADDITIONS -----------

// Type transformations
template<typename T>
void transform_type() {
    using no_const = typename std::remove_const<T>::type;
    using no_ref   = typename std::remove_reference<T>::type;

    std::cout << "Removed const? "
              << std::is_const<no_const>::value << "\n";

    std::cout << "Removed reference? "
              << std::is_reference<no_ref>::value << "\n";
}

// Advanced traits
template<typename T>
void advanced_traits() {
    std::cout << "Trivially copyable: "
              << std::is_trivially_copyable<T>::value << "\n";

    std::cout << "Standard layout: "
              << std::is_standard_layout<T>::value << "\n";
}

// Padding demo struct
struct PaddingDemo {
    char a;
    int b;
    char c;
};

// aligned_storage example
void aligned_storage_demo() {
    using Storage = std::aligned_storage<sizeof(int), alignof(int)>::type;

    Storage storage;
    int* ptr = reinterpret_cast<int*>(&storage);
    *ptr = 42;

    std::cout << "Aligned storage value: " << *ptr << "\n";
}

// ----------- MORE ADVANCED ADDITIONS -----------

// Type decay demo
template<typename T>
void decay_demo() {
    using Decayed = typename std::decay<T>::type;

    std::cout << "Original is reference? "
              << std::is_reference<T>::value << "\n";

    std::cout << "After decay is reference? "
              << std::is_reference<Decayed>::value << "\n";
}

// Enable_if example (only for integral types)
template<typename T>
typename std::enable_if<std::is_integral<T>::value>::type
only_integral(T val) {
    std::cout << "Integral value: " << val << "\n";
}

// Detect if type is class
template<typename T>
void check_class() {
    std::cout << "Is class type: "
              << std::is_class<T>::value << "\n";
}

// Reference category checker
template<typename T>
void reference_category(T&&) {
    if constexpr (std::is_lvalue_reference<T>::value) {
        std::cout << "Lvalue reference detected\n";
    } else {
        std::cout << "Rvalue reference detected\n";
    }
}

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

    // -------- EXTRA FEATURE USAGE --------

    std::cout << "\n--- Type Transformations ---\n";
    transform_type<const int&>();

    std::cout << "\n--- Advanced Traits ---\n";
    advanced_traits<int>();
    advanced_traits<AlignedStruct>();

    std::cout << "\n--- Padding Demo ---\n";
    std::cout << "Size of PaddingDemo: "
              << sizeof(PaddingDemo) << "\n";

    std::cout << "Alignment of PaddingDemo: "
              << alignof(PaddingDemo) << "\n";

    std::cout << "\n--- Aligned Storage Demo ---\n";
    aligned_storage_demo();

    // -------- MORE ADVANCED USAGE --------

    std::cout << "\n--- Decay Demo ---\n";
    decay_demo<const int&>();

    std::cout << "\n--- enable_if Demo ---\n";
    only_integral(10);
    // only_integral(3.14); // ❌ won't compile

    std::cout << "\n--- Class Detection ---\n";
    check_class<int>();
    check_class<AlignedStruct>();

    std::cout << "\n--- Reference Category ---\n";
    int x = 5;
    reference_category(x);       // lvalue
    reference_category(10);      // rvalue

    // Compile-time check
    static_assert(sizeof(int) >= 4, "Unexpected int size!");

    // ----------------------------------

    return 0;
}
