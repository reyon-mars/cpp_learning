// Template Specialization Exercise
// Full and partial specialization

#include <iostream>
#include <vector>
#include <type_traits> // 🔹 ADDED

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

// 🔹 NEW: const pointer specialization
template<typename T>
struct traits<const T*> {
    static constexpr const char* name = "const pointer";
    static void print() {
        std::cout << "Specialized for const pointers\n";
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

// ---------------- SMALL ADDITIONS ----------------

template<typename T>
void print_trait_name() {
    std::cout << "Trait name: " << traits<T>::name << "\n";
}

template<typename T>
void test_traits() {
    traits<T>::print();
    print_trait_name<T>();
    std::cout << "------------------\n";
}

template<typename T>
struct traits<T&> {
    static constexpr const char* name = "reference";
    static void print() {
        std::cout << "Specialized for reference types\n";
    }
};

// ======================================================
// 🔥 NEW ADDITIONS
// ======================================================

// 🔹 Array specialization
template<typename T, std::size_t N>
struct traits<T[N]> {
    static constexpr const char* name = "array";
    static void print() {
        std::cout << "Specialized for array types\n";
    }
};

// 🔹 Function type specialization
template<typename R, typename... Args>
struct traits<R(Args...)> {
    static constexpr const char* name = "function";
    static void print() {
        std::cout << "Specialized for function types\n";
    }
};

// 🔹 Detection idiom: has value_type
template<typename, typename = std::void_t<>>
struct has_value_type : std::false_type {};

template<typename T>
struct has_value_type<T, std::void_t<typename T::value_type>>
    : std::true_type {};

// 🔹 Conditional printer using detection
template<typename T>
void print_value_type_info() {
    if constexpr (has_value_type<T>::value) {
        std::cout << "Has value_type\n";
    } else {
        std::cout << "No value_type\n";
    }
}

// 🔹 Compile-time category checker
template<typename T>
void category_check() {
    if constexpr (std::is_pointer_v<T>)
        std::cout << "Category: Pointer\n";
    else if constexpr (std::is_reference_v<T>)
        std::cout << "Category: Reference\n";
    else if constexpr (std::is_array_v<T>)
        std::cout << "Category: Array\n";
    else
        std::cout << "Category: Other\n";
}

// ======================================================
// Main
// ======================================================
int main() {

    traits<double>::print();
    traits<int>::print();
    traits<double*>::print();
    traits<const float>::print();
    traits<std::vector<double>>::print();
    traits<std::vector<int>>::print();

    std::cout << "\n--- Extra Tests ---\n";

    test_traits<int>();
    test_traits<double*>();
    test_traits<const int>();
    test_traits<std::vector<float>>();
    test_traits<std::vector<int>>();
    test_traits<int&>();

    // ======================================================
    // 🔥 NEW USAGE
    // ======================================================

    std::cout << "\n--- Advanced Specialization Tests ---\n";

    int arr[5];
    test_traits<decltype(arr)>();

    test_traits<void(int)>(); // function type

    print_value_type_info<std::vector<int>>();
    print_value_type_info<int>();

    category_check<int*>();
    category_check<int&>();
    category_check<decltype(arr)>();

    // ======================================================

    return 0;
}
