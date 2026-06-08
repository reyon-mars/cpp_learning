#include <iostream>
#include <type_traits>
#include <string>
#include <string_view>
#include <vector>
#include <cassert>
#include <utility>
#include <concepts>

template<typename T>
struct is_string : std::bool_constant<std::is_same_v<T, std::string>> {};

template<typename T>
inline constexpr bool is_string_v = is_string<T>::value;

template<typename T, typename Alloc>
struct is_vector_spec : std::false_type {};

template<typename T, typename Alloc>
struct is_vector_spec<std::vector<T, Alloc>, Alloc> : std::true_type {};

template<typename T>
struct is_vector : std::false_type {};

template<typename T, typename Alloc>
struct is_vector<std::vector<T, Alloc>> : std::true_type {};

template<typename T>
inline constexpr bool is_vector_v = is_vector<T>::value;

template<typename T>
struct pointer_depth : std::integral_constant<int, 0> {};

template<typename T>
struct pointer_depth<T*> : std::integral_constant<int, 1 + pointer_depth<T>::value> {};

template<typename T>
inline constexpr int pointer_depth_v = pointer_depth<T>::value;

static_assert(is_string_v<std::string>);
static_assert(!is_string_v<int>);
static_assert(is_vector_v<std::vector<int>>);
static_assert(!is_vector_v<int>);
static_assert(pointer_depth_v<int***> == 3);
static_assert(std::is_same_v<int, int>);
static_assert(!std::is_same_v<int, double>);

template<typename T>
void analyze_type() {
    std::cout << std::boolalpha
              << "  is_integral="   << std::is_integral_v<T>   << "\n"
              << "  is_pointer="    << std::is_pointer_v<T>    << "\n"
              << "  is_class="      << std::is_class_v<T>      << "\n"
              << "  is_trivial="    << std::is_trivial_v<T>    << "\n"
              << "  is_const="      << std::is_const_v<T>      << "\n"
              << "  is_reference="  << std::is_reference_v<T>  << "\n"
              << "  is_arithmetic=" << std::is_arithmetic_v<T> << "\n"
              << "  is_string="     << is_string_v<T>           << "\n"
              << "  sizeof="        << sizeof(T)                << "\n";

    if constexpr (std::is_pointer_v<T>)
        std::cout << "  -> pointer\n";
    else if constexpr (std::is_integral_v<T>)
        std::cout << "  -> integral\n";
    else if constexpr (std::is_class_v<T>)
        std::cout << "  -> class\n";
}

template<typename T>
void analyze_decay() {
    using D = std::decay_t<T>;
    std::cout << std::boolalpha
              << "  is_same<T, decay_t<T>>=" << std::is_same_v<T, D> << "\n"
              << "  sizeof(decay_t<T>)="      << sizeof(D)             << "\n";
}

template<typename T>
    requires std::is_integral_v<T>
void only_integral(T value) {
    std::cout << "Integral-only: " << value << "\n";
}

template<typename T>
void extra_traits() {
    std::cout << std::boolalpha
              << "  is_array=" << std::is_array_v<T>                                             << "\n"
              << "  is_enum="  << std::is_enum_v<T>                                              << "\n"
              << "  is_const(no_ref)="
              << std::is_const_v<std::remove_reference_t<T>>                                     << "\n";
}

template<typename T>
void pointer_details() {
    if constexpr (std::is_pointer_v<T>) {
        std::cout << "  base sizeof="
                  << sizeof(std::remove_pointer_t<T>) << "\n";
    }
}

template<typename T>
void reference_info() {
    std::cout << std::boolalpha
              << "  is_lvalue_reference=" << std::is_lvalue_reference_v<T> << "\n"
              << "  is_rvalue_reference=" << std::is_rvalue_reference_v<T> << "\n";
}

template<typename T>
void signed_info() {
    std::cout << std::boolalpha
              << "  is_signed="   << std::is_signed_v<T>   << "\n"
              << "  is_unsigned=" << std::is_unsigned_v<T> << "\n";
}

template<typename T>
void object_capabilities() {
    std::cout << std::boolalpha
              << "  copy_constructible="    << std::is_copy_constructible_v<T>    << "\n"
              << "  move_constructible="    << std::is_move_constructible_v<T>    << "\n"
              << "  default_constructible=" << std::is_default_constructible_v<T> << "\n";
}

template<typename T>
void function_check() {
    std::cout << std::boolalpha
              << "  is_function=" << std::is_function_v<T> << "\n";
}

int main() {
    std::cout << "=== int ===\n";
    analyze_type<int>();

    std::cout << "\n=== int* ===\n";
    analyze_type<int*>();

    std::cout << "\n=== const double& ===\n";
    analyze_type<const double&>();

    std::cout << "\n=== std::string ===\n";
    analyze_type<std::string>();

    std::cout << "\n=== analyze_decay ===\n";
    analyze_decay<const int&>();
    analyze_decay<int[5]>();

    std::cout << "\n=== pointer_depth ===\n";
    std::cout << "int="   << pointer_depth_v<int>   << "\n"
              << "int*="  << pointer_depth_v<int*>  << "\n"
              << "int**=" << pointer_depth_v<int**> << "\n";

    std::cout << "\n=== only_integral ===\n";
    only_integral(100);

    std::cout << "\n=== extra_traits ===\n";
    extra_traits<int>();
    extra_traits<const int&>();
    extra_traits<int[5]>();

    std::cout << "\n=== pointer_details ===\n";
    pointer_details<int*>();
    pointer_details<double*>();

    std::cout << "\n=== reference_info ===\n";
    reference_info<int>();
    reference_info<int&>();
    reference_info<int&&>();

    std::cout << "\n=== is_vector ===\n";
    std::cout << "int="              << is_vector_v<int>              << "\n"
              << "vector<int>="      << is_vector_v<std::vector<int>> << "\n";

    std::cout << "\n=== signed_info ===\n";
    signed_info<int>();
    signed_info<unsigned int>();

    std::cout << "\n=== object_capabilities ===\n";
    object_capabilities<std::string>();

    std::cout << "\n=== function_check ===\n";
    function_check<int>();
    function_check<void()>();

    assert(is_vector_v<std::vector<int>>);

    return 0;
}
