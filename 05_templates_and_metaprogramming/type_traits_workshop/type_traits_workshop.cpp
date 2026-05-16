// Type Traits Workshop
// Compile-time type introspection

#include <iostream>
#include <type_traits>
#include <string>
#include <vector>     // 🔹 ADDED
#include <cassert>    // 🔹 ADDED
#include <utility>    // 🔹 ADDED

// ----------------------------------
// Custom type trait example
// ----------------------------------
template<typename T>
struct is_string : std::false_type {};

template<>
struct is_string<std::string> : std::true_type {};

// ----------------------------------
// Type analyzer
// ----------------------------------
template<typename T>
void analyze_type() {

    std::cout << "Type analysis:\n";

    std::cout << "  is_integral: "
              << std::is_integral_v<T> << "\n";

    std::cout << "  is_pointer: "
              << std::is_pointer_v<T> << "\n";

    std::cout << "  is_class: "
              << std::is_class_v<T> << "\n";

    std::cout << "  is_trivial: "
              << std::is_trivial_v<T> << "\n";

    std::cout << "  is_const: "
              << std::is_const_v<T> << "\n";

    std::cout << "  is_reference: "
              << std::is_reference_v<T> << "\n";

    std::cout << "  is_arithmetic: "
              << std::is_arithmetic_v<T> << "\n";

    std::cout << "  is_string (custom): "
              << is_string<T>::value << "\n";

    std::cout << "  size: "
              << sizeof(T) << "\n";

    // Compile-time branching
    if constexpr (std::is_pointer_v<T>) {
        std::cout << "  -> This is a pointer type\n";
    }
    else if constexpr (std::is_integral_v<T>) {
        std::cout << "  -> This is an integral type\n";
    }
    else if constexpr (std::is_class_v<T>) {
        std::cout << "  -> This is a class type\n";
    }
}

// ---------------- SMALL ADDITIONS ----------------

// Remove const/reference helper demo
template<typename T>
void analyze_decay() {

    using Decayed = std::decay_t<T>;

    std::cout << "Decayed type info:\n";

    std::cout << "  is_same<T, decay_t<T>>: "
              << std::is_same_v<T, Decayed> << "\n";

    // 🔹 NEW: show size after decay
    std::cout << "  size of decayed type: "
              << sizeof(Decayed) << "\n";
}

// Pointer depth checker
template<typename T>
struct pointer_depth
    : std::integral_constant<int, 0> {};

template<typename T>
struct pointer_depth<T*>
    : std::integral_constant<
          int,
          1 + pointer_depth<T>::value> {};

// Print pointer depth
template<typename T>
void print_pointer_depth() {

    std::cout << "Pointer depth: "
              << pointer_depth<T>::value
              << "\n";
}

// Enable_if usage example
template<typename T>
std::enable_if_t<std::is_integral_v<T>, void>
only_integral(T value) {

    std::cout << "Integral-only function: "
              << value << "\n";
}

// ----------- NEW ADDITIONS -----------

// Extra trait checks
template<typename T>
void extra_traits() {

    std::cout << "Extra traits:\n";

    std::cout << "  is_array: "
              << std::is_array_v<T> << "\n";

    std::cout << "  is_enum: "
              << std::is_enum_v<T> << "\n";

    // Better const detection (remove reference)
    std::cout << "  is_const (no ref): "
              << std::is_const_v<
                     std::remove_reference_t<T>>
              << "\n";
}

// Pointer base type info
template<typename T>
void pointer_details() {

    if constexpr (std::is_pointer_v<T>) {

        std::cout << "Pointer base size: "
                  << sizeof(std::remove_pointer_t<T>)
                  << "\n";
    }
}

// Reference category detection
template<typename T>
void reference_info() {

    std::cout << "Reference info:\n";

    std::cout << "  is_lvalue_reference: "
              << std::is_lvalue_reference_v<T>
              << "\n";

    std::cout << "  is_rvalue_reference: "
              << std::is_rvalue_reference_v<T>
              << "\n";
}

// ======================================================
// EXTRA SMALL ADDITIONS
// ======================================================

// Detect vector type
template<typename T>
struct is_vector : std::false_type {};

template<typename T, typename Alloc>
struct is_vector<std::vector<T, Alloc>>
    : std::true_type {};

// Print signed/unsigned info
template<typename T>
void signed_info() {

    std::cout << "Signed info:\n";

    std::cout << "  is_signed: "
              << std::is_signed_v<T> << "\n";

    std::cout << "  is_unsigned: "
              << std::is_unsigned_v<T> << "\n";
}

// Check movable/copyable
template<typename T>
void object_capabilities() {

    std::cout << "Object capabilities:\n";

    std::cout << "  is_copy_constructible: "
              << std::is_copy_constructible_v<T>
              << "\n";

    std::cout << "  is_move_constructible: "
              << std::is_move_constructible_v<T>
              << "\n";

    std::cout << "  is_default_constructible: "
              << std::is_default_constructible_v<T>
              << "\n";
}

// Detect function types
template<typename T>
void function_check() {

    std::cout << "Function traits:\n";

    std::cout << "  is_function: "
              << std::is_function_v<T>
              << "\n";
}

// ======================================================
// Main
// ======================================================
int main() {

    static_assert(std::is_same_v<int, int>);
    static_assert(!std::is_same_v<int, double>);

    std::cout << "Analyzing int:\n";
    analyze_type<int>();

    std::cout << "\nAnalyzing int*:\n";
    analyze_type<int*>();

    std::cout << "\nAnalyzing const double&:\n";
    analyze_type<const double&>();

    std::cout << "\nAnalyzing std::string:\n";
    analyze_type<std::string>();

    // -------- Added usage --------
    std::cout << "\n--- Extra Tests ---\n";

    analyze_decay<const int&>();

    print_pointer_depth<int>();
    print_pointer_depth<int*>();
    print_pointer_depth<int**>();

    only_integral(100);
    // only_integral(3.14); // would fail at compile-time

    // -------- NEW USAGE --------

    std::cout << "\n--- More Trait Analysis ---\n";

    extra_traits<int>();
    extra_traits<const int&>();
    extra_traits<int[5]>();

    pointer_details<int*>();
    pointer_details<double*>();

    reference_info<int>();
    reference_info<int&>();
    reference_info<int&&>();

    // ======================================================
    // EXTRA SMALL USAGE
    // ======================================================

    std::cout << "\n--- Additional Traits ---\n";

    std::cout << "is_vector<int>: "
              << is_vector<int>::value << "\n";

    std::cout << "is_vector<std::vector<int>>: "
              << is_vector<std::vector<int>>::value
              << "\n";

    signed_info<int>();
    signed_info<unsigned int>();

    object_capabilities<std::string>();

    function_check<int>();
    function_check<void()>();

    // ✅ Compile-time validation
    static_assert(is_string<std::string>::value);
    static_assert(pointer_depth<int***>::value == 3);

    // ✅ Runtime validation
    assert(is_vector<std::vector<int>>::value);

    // ======================================================

    return 0;
}
