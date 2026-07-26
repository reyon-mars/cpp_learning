#include <concepts>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <type_traits>
#include <typeinfo>
#include <utility>

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
    using NoCvRef = std::remove_cvref_t<T>;

    std::cout << "After remove_const, still const?    " << std::is_const_v<NoConst> << "\n"
              << "After remove_reference, still ref?  " << std::is_reference_v<NoRef> << "\n"
              << "After remove_cvref, still const?    " << std::is_const_v<NoCvRef> << "\n"
              << "After remove_cvref, still ref?      " << std::is_reference_v<NoCvRef> << "\n";
}

void const_reference_gotcha_demo() {
    std::cout << "is_const_v<const int&> (a reference itself is never const-qualified): "
              << std::is_const_v<const int&> << "\n"
              << "remove_const_t<const int&> leaves the reference untouched, still const-looking source: "
              << std::is_same_v<std::remove_const_t<const int&>, const int&> << "\n"
              << "remove_reference_t first, then is_const_v sees the real const: "
              << std::is_const_v<std::remove_reference_t<const int&>> << "\n";
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

template<std::integral T>
void only_integral_concept(T val) {
    std::cout << "Integral value (concept-constrained): " << val << "\n";
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

template<typename T>
concept Numeric = std::is_arithmetic_v<T>;

template<Numeric T>
[[nodiscard]] constexpr T double_it(T val) noexcept {
    return val + val;
}

struct Base {
    virtual ~Base() = default;
};
struct Derived : Base {};
struct Unrelated {};

void inheritance_traits_demo() {
    std::cout << "is_base_of<Base, Derived>:    " << std::is_base_of_v<Base, Derived>    << "\n"
              << "is_base_of<Base, Unrelated>:  " << std::is_base_of_v<Base, Unrelated>  << "\n"
              << "derived_from<Derived, Base>:  " << std::derived_from<Derived, Base>    << "\n";
}

enum class Suit : unsigned char { Clubs, Diamonds, Hearts, Spades };

void underlying_type_demo() {
    using Raw = std::underlying_type_t<Suit>;
    std::cout << "Suit underlying_type is same as unsigned char: " << std::is_same_v<Raw, unsigned char> << "\n"
              << "Suit::Hearts as underlying value: " << static_cast<int>(static_cast<Raw>(Suit::Hearts)) << "\n";
}

void common_type_demo() {
    using Common = std::common_type_t<int, double, char>;
    std::cout << "common_type_t<int,double,char> is double: " << std::is_same_v<Common, double> << "\n";
}

void invoke_result_demo() {
    auto add = [](int a, double b) { return a + b; };
    using Result = std::invoke_result_t<decltype(add), int, double>;
    std::cout << "invoke_result_t of (int,double)->auto is double: " << std::is_same_v<Result, double> << "\n"
              << "invoked value: " << std::invoke(add, 3, 4.5) << "\n";
}

void conditional_type_demo() {
    using Selected = std::conditional_t<(sizeof(long) > sizeof(int)), long, int>;
    std::cout << "conditional_t picked long over int: " << std::is_same_v<Selected, long> << "\n";
}

int main() {
    static_assert(sizeof(int) >= 4, "Unexpected int size");

    std::cout << "--- print_traits<int> ---\n";
    print_traits<int>();

    std::cout << "\n--- print_traits<int*> ---\n";
    print_traits<int*>();

    alignas(64) char buffer[256];
    std::cout << "\nalignof(decltype(buffer)) reports: " << alignof(decltype(buffer))
              << " (decltype does not carry the alignas from this declaration into the array type)\n"
              << "actual runtime address is 64-byte aligned: "
              << std::boolalpha << (reinterpret_cast<std::uintptr_t>(&buffer) % 64 == 0) << "\n";

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

    std::cout << "\n--- The const-reference gotcha ---\n";
    const_reference_gotcha_demo();

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
    only_integral_concept(20);

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

    std::cout << "\n--- is_numeric_v / Numeric concept ---\n"
              << "int:         " << is_numeric_v<int>         << "\n"
              << "std::string: " << is_numeric_v<std::string> << "\n"
              << "double_it(21) via Numeric-constrained function: " << double_it(21) << "\n";

    std::cout << "\n--- Inheritance traits ---\n";
    inheritance_traits_demo();

    std::cout << "\n--- Underlying type ---\n";
    underlying_type_demo();

    std::cout << "\n--- Common type ---\n";
    common_type_demo();

    std::cout << "\n--- Invoke result ---\n";
    invoke_result_demo();

    std::cout << "\n--- Conditional type ---\n";
    conditional_type_demo();

    return 0;
}
