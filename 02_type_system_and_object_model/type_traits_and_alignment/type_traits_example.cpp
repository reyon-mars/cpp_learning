#include <iostream>
#include <type_traits>
#include <utility>
#include <typeinfo>
#include <string>
#include <memory>

template<typename T>
void print_traits() {
    std::cout << "Is integral:  " << std::is_integral_v<T>  << "\n"
              << "Is pointer:   " << std::is_pointer_v<T>   << "\n"
              << "Size:         " << sizeof(T)               << "\n"
              << "Alignment:    " << alignof(T)              << "\n";
}

template<typename T>
void print_more_traits() {
    std::cout << "Is const:       " << std::is_const_v<T>          << "\n"
              << "Is reference:   " << std::is_reference_v<T>      << "\n"
              << "Is same as int: " << std::is_same_v<T, int>      << "\n";
}

template<typename T>
void analyze_type() {
    if constexpr (std::is_integral_v<T>) {
        std::cout << "Integral type\n";
    } else if constexpr (std::is_floating_point_v<T>) {
        std::cout << "Floating-point type\n";
    } else if constexpr (std::is_class_v<T>) {
        std::cout << "Class type\n";
    } else {
        std::cout << "Other type\n";
    }
}

struct alignas(32) AlignedStruct {
    int    x;
    double y;
};

struct PaddingDemo {
    char a;
    int  b;
    char c;
};

template<typename T>
void transform_type() {
    using NoConst = std::remove_const_t<T>;
    using NoRef   = std::remove_reference_t<T>;

    std::cout << "After remove_const, still const?    " << std::is_const_v<NoConst> << "\n"
              << "After remove_reference, still ref?  " << std::is_reference_v<NoRef> << "\n";
}

template<typename T>
void advanced_traits() {
    std::cout << "Trivially copyable:  " << std::is_trivially_copyable_v<T> << "\n"
              << "Standard layout:     " << std::is_standard_layout_v<T>    << "\n";
}

void aligned_storage_demo() {
    alignas(alignof(int)) std::byte storage[sizeof(int)];
    auto* ptr = new (&storage) int{42};
    std::cout << "Aligned storage value: " << *ptr << "\n";
    std::destroy_at(ptr);
}

template<typename T>
void decay_demo() {
    using Decayed = std::decay_t<T>;
    std::cout << "Original is reference?      " << std::is_reference_v<T>       << "\n"
              << "After decay is reference?   " << std::is_reference_v<Decayed> << "\n"
              << "After decay is pointer?     " << std::is_pointer_v<Decayed>   << "\n";
}

template<typename T>
    requires std::is_integral_v<T>
void only_integral(T val) {
    std::cout << "Integral value: " << val << "\n";
}

template<typename T>
void reference_category(T&&) {
    if constexpr (std::is_lvalue_reference_v<T>) {
        std::cout << "Lvalue reference\n";
    } else {
        std::cout << "Rvalue reference\n";
    }
}

template<typename T>
void remove_pointer_demo() {
    using Base = std::remove_pointer_t<T>;
    std::cout << "Original is pointer?   " << std::is_pointer_v<T>    << "\n"
              << "Base type is pointer?  " << std::is_pointer_v<Base> << "\n";
}

template<typename T>
void print_type_name() {
    std::cout << "Type name: " << typeid(T).name() << "\n";
}

template<typename T>
void move_demo(T&& value) {
    [[maybe_unused]] T moved = std::forward<T>(value);
    std::cout << "Perfect-forward executed\n";
}

template<typename T>
inline constexpr bool is_numeric_v = std::is_arithmetic_v<T>;

int main() {
    static_assert(sizeof(int) >= 4, "Unexpected int size");

    std::cout << "--- print_traits<int> ---\n";
    print_traits<int>();

    std::cout << "\n--- print_traits<int*> ---\n";
    print_traits<int*>();

    alignas(64) char buffer[256];
    std::cout << "\nBuffer alignment: " << alignof(decltype(buffer)) << "\n";

    std::cout << "\n--- More traits: int ---\n";
    print_more_traits<int>();

    std::cout << "\n--- More traits: const int& ---\n";
    print_more_traits<const int&>();

    std::cout << "\n--- analyze_type ---\n";
    analyze_type<int>();
    analyze_type<double>();
    analyze_type<AlignedStruct>();

    std::cout << "\n--- AlignedStruct ---\n"
              << "Size:      " << sizeof(AlignedStruct)              << "\n"
              << "Alignment: " << alignof(AlignedStruct)             << "\n"
              << "alignment_of<int>: " << std::alignment_of_v<int>  << "\n";

    std::cout << "\n--- Type Transformations ---\n";
    transform_type<const int&>();

    std::cout << "\n--- Advanced Traits ---\n";
    advanced_traits<int>();
    advanced_traits<AlignedStruct>();

    std::cout << "\n--- PaddingDemo ---\n"
              << "Size:      " << sizeof(PaddingDemo)  << "\n"
              << "Alignment: " << alignof(PaddingDemo) << "\n";

    std::cout << "\n--- Aligned Storage ---\n";
    aligned_storage_demo();

    std::cout << "\n--- Decay Demo ---\n";
    decay_demo<const int&>();
    decay_demo<int[3]>();

    std::cout << "\n--- requires constraint (only_integral) ---\n";
    only_integral(10);

    std::cout << "\n--- Reference Category ---\n";
    int x = 5;
    reference_category(x);
    reference_category(42);

    std::cout << "\n--- Floating Point Check ---\n";
    std::cout << "float is floating_point: " << std::is_floating_point_v<float> << "\n"
              << "int is floating_point:   " << std::is_floating_point_v<int>   << "\n";

    std::cout << "\n--- Remove Pointer ---\n";
    remove_pointer_demo<int*>();

    std::cout << "\n--- Type Name ---\n";
    print_type_name<double>();

    std::cout << "\n--- Move/Forward Demo ---\n";
    std::string text = "Hello";
    move_demo(std::move(text));

    std::cout << "\n--- is_numeric_v ---\n"
              << "int:         " << is_numeric_v<int>         << "\n"
              << "std::string: " << is_numeric_v<std::string> << "\n";

    return 0;
}
