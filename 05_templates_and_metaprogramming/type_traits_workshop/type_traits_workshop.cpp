#include <iostream>
#include <type_traits>
#include <string>
#include <string_view>
#include <vector>
#include <map>
#include <array>
#include <memory>
#include <cassert>
#include <utility>
#include <concepts>

template<typename T>
struct is_string : std::bool_constant<std::is_same_v<T, std::string>> {};

template<typename T>
inline constexpr bool is_string_v = is_string<T>::value;

template<typename T>
struct is_vector : std::false_type {};

template<typename T, typename Alloc>
struct is_vector<std::vector<T, Alloc>> : std::true_type {};

template<typename T>
inline constexpr bool is_vector_v = is_vector<T>::value;

template<typename T>
struct is_map : std::false_type {};

template<typename K, typename V, typename Cmp, typename Alloc>
struct is_map<std::map<K, V, Cmp, Alloc>> : std::true_type {};

template<typename T>
inline constexpr bool is_map_v = is_map<T>::value;

template<typename T>
struct is_array_like : std::false_type {};

template<typename T, std::size_t N>
struct is_array_like<std::array<T, N>> : std::true_type {};

template<typename T>
inline constexpr bool is_array_like_v = is_array_like<T>::value;

template<typename T>
struct is_smart_pointer : std::false_type {};

template<typename T>
struct is_smart_pointer<std::unique_ptr<T>> : std::true_type {};

template<typename T>
struct is_smart_pointer<std::shared_ptr<T>> : std::true_type {};

template<typename T>
inline constexpr bool is_smart_pointer_v = is_smart_pointer<T>::value;

template<typename T>
struct pointer_depth : std::integral_constant<int, 0> {};

template<typename T>
struct pointer_depth<T*> : std::integral_constant<int, 1 + pointer_depth<T>::value> {};

template<typename T>
inline constexpr int pointer_depth_v = pointer_depth<T>::value;

template<typename T>
struct array_extent_product : std::integral_constant<std::size_t, 1> {};

template<typename T, std::size_t N>
struct array_extent_product<T[N]>
    : std::integral_constant<std::size_t, N * array_extent_product<T>::value> {};

template<typename T>
inline constexpr std::size_t array_extent_product_v = array_extent_product<T>::value;

static_assert(is_string_v<std::string>);
static_assert(!is_string_v<int>);
static_assert(is_vector_v<std::vector<int>>);
static_assert(!is_vector_v<int>);
static_assert(is_map_v<std::map<int, std::string>>);
static_assert(!is_map_v<std::vector<int>>);
static_assert(is_array_like_v<std::array<int, 5>>);
static_assert(!is_array_like_v<std::vector<int>>);
static_assert(is_smart_pointer_v<std::unique_ptr<int>>);
static_assert(is_smart_pointer_v<std::shared_ptr<int>>);
static_assert(!is_smart_pointer_v<int*>);
static_assert(pointer_depth_v<int***> == 3);
static_assert(array_extent_product_v<int[3][4]> == 12);
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
              << "  is_array=" << std::is_array_v<T>                          << "\n"
              << "  is_enum="  << std::is_enum_v<T>                           << "\n"
              << "  is_const(no_ref)="
              << std::is_const_v<std::remove_reference_t<T>>                  << "\n";
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
              << "  default_constructible=" << std::is_default_constructible_v<T> << "\n"
              << "  nothrow_move_constructible=" << std::is_nothrow_move_constructible_v<T> << "\n";
}

template<typename T>
void function_check() {
    std::cout << std::boolalpha
              << "  is_function=" << std::is_function_v<T> << "\n";
}

template<typename Base, typename Derived>
void inheritance_check() {
    std::cout << std::boolalpha
              << "  is_base_of=" << std::is_base_of_v<Base, Derived> << "\n";
}

template<typename From, typename To>
void conversion_check() {
    std::cout << std::boolalpha
              << "  is_convertible=" << std::is_convertible_v<From, To> << "\n";
}

template<typename T>
[[nodiscard]] constexpr auto remove_all_pointers() {
    if constexpr (std::is_pointer_v<T>)
        return std::type_identity<typename std::remove_pointer_t<T>>{};
    else
        return std::type_identity<T>{};
}

template<typename T>
using remove_all_pointers_t = typename decltype(remove_all_pointers<T>())::type;

static_assert(std::is_same_v<remove_all_pointers_t<int*>, int>);
static_assert(std::is_same_v<remove_all_pointers_t<int>,  int>);

struct Base {};
struct Derived : Base {};

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

    std::cout << "\n=== is_vector / is_map / is_array_like ===\n";
    std::cout << "int="              << is_vector_v<int>                 << "\n"
              << "vector<int>="      << is_vector_v<std::vector<int>>    << "\n"
              << "map<int,string>="  << is_map_v<std::map<int, std::string>> << "\n"
              << "array<int,5>="     << is_array_like_v<std::array<int, 5>> << "\n";

    std::cout << "\n=== is_smart_pointer ===\n";
    std::cout << "unique_ptr<int>=" << is_smart_pointer_v<std::unique_ptr<int>> << "\n"
              << "shared_ptr<int>=" << is_smart_pointer_v<std::shared_ptr<int>> << "\n"
              << "int*="            << is_smart_pointer_v<int*>                 << "\n";

    std::cout << "\n=== array_extent_product ===\n";
    std::cout << "int[3][4]=" << array_extent_product_v<int[3][4]> << "\n";

    std::cout << "\n=== signed_info ===\n";
    signed_info<int>();
    signed_info<unsigned int>();

    std::cout << "\n=== object_capabilities ===\n";
    object_capabilities<std::string>();

    std::cout << "\n=== function_check ===\n";
    function_check<int>();
    function_check<void()>();

    std::cout << "\n=== inheritance_check / conversion_check ===\n";
    inheritance_check<Base, Derived>();
    inheritance_check<Derived, Base>();
    conversion_check<int, double>();
    conversion_check<std::string, int>();

    std::cout << "\n=== remove_all_pointers_t ===\n";
    std::cout << std::boolalpha
              << "remove_all_pointers_t<int*> == int? "
              << std::is_same_v<remove_all_pointers_t<int*>, int> << "\n";

    assert(is_vector_v<std::vector<int>>);
    using IntIntMap = std::map<int, int>;
    assert(is_map_v<IntIntMap>);
    assert(is_smart_pointer_v<std::shared_ptr<double>>);

    std::cout << "\nAll assertions passed.\n";
    return 0;
}
