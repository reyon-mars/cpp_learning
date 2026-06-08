#include <iostream>
#include <type_traits>
#include <vector>
#include <string>
#include <string_view>
#include <iterator>
#include <list>
#include <map>
#include <cassert>
#include <utility>

template<typename T>
concept Arithmetic = std::is_arithmetic_v<T>;

template<typename T>
concept Iterable = requires(T t) {
    { std::begin(t) };
    { std::end(t)   };
};

template<typename T>
concept Streamable = requires(std::ostream& os, T t) {
    { os << t } -> std::same_as<std::ostream&>;
};

template<typename T>
concept HasSize = requires(const T& t) {
    { t.size() } -> std::convertible_to<std::size_t>;
};

template<typename T>
concept HasPushBack = requires(T t, typename T::value_type v) {
    { t.push_back(v) };
};

template<Arithmetic T>
void process(T value) {
    if constexpr (std::is_integral_v<T>)
        std::cout << "Integral: " << value << "\n";
    else
        std::cout << "Float: " << value << "\n";
}

template<typename T>
    requires std::is_pointer_v<T>
void process_pointer(T value) {
    std::cout << "Pointer: " << static_cast<const void*>(value) << "\n";
}

template<HasSize T>
void process_container(const T& value) {
    std::cout << "Container size=" << value.size() << "\n";
}

template<Arithmetic T>
void process_v2(T value) {
    std::cout << "process_v2: " << value << "\n";
}

template<Iterable T>
void process_iterable(const T& container) {
    std::cout << "Iterable: ";
    for (const auto& x : container) std::cout << x << " ";
    std::cout << "\n";
}

template<HasSize T>
void process_requires(const T& value) {
    std::cout << "process_requires size=" << value.size() << "\n";
}

template<Streamable T>
void print_streamable(const T& value) {
    std::cout << "Streamable: " << value << "\n";
}

template<typename T>
void smart_process(const T& value) {
    if constexpr (Iterable<T>)
        process_iterable(value);
    else if constexpr (Arithmetic<T>)
        process_v2(value);
    else
        std::cout << "Unknown type\n";
}

template<typename T>
void safe_process(const T& value) {
    if constexpr (std::is_integral_v<T>)
        process(value);
    else if constexpr (std::is_floating_point_v<T>)
        process(value);
    else
        std::cout << "Processing generic type\n";
}

template<typename T>
void no_deduction(typename std::type_identity<T>::type value) {
    std::cout << "type_identity: " << value << "\n";
}

template<typename T>
void print_type_info() {
    std::cout << "integral="  << std::boolalpha << std::is_integral_v<T>         << " "
              << "floating="  << std::is_floating_point_v<T> << " "
              << "pointer="   << std::is_pointer_v<T>        << "\n";
}

template<typename T>
void check_push_back_support() {
    if constexpr (HasPushBack<T>)
        std::cout << "supports push_back\n";
    else
        std::cout << "does NOT support push_back\n";
}

template<typename T>
void check_pointer() {
    if constexpr (std::is_pointer_v<T>)
        std::cout << "pointer type\n";
    else
        std::cout << "not a pointer type\n";
}

template<Arithmetic T>
[[nodiscard]] constexpr T add_values(T a, T b) noexcept { return a + b; }

static_assert(add_values(2, 3) == 5);
static_assert(add_values(1.5, 2.5) == 4.0);

int main() {
    std::cout << "=== process (integral / float) ===\n";
    process(42);
    process(3.14);

    std::cout << "\n=== process_pointer ===\n";
    int x = 10;
    process_pointer(&x);

    std::cout << "\n=== process_container ===\n";
    const std::vector<int> vec{1, 2, 3};
    process_container(vec);

    std::cout << "\n=== process_v2 ===\n";
    process_v2(100);
    process_v2(2.71);

    std::cout << "\n=== safe_process ===\n";
    safe_process(50);
    safe_process(5.5);
    safe_process(std::string{"hello"});

    std::cout << "\n=== print_type_info ===\n";
    print_type_info<int>();
    print_type_info<double>();
    print_type_info<int*>();

    std::cout << "\n=== print_streamable ===\n";
    print_streamable(123);
    print_streamable(std::string{"SFINAE"});

    std::cout << "\n=== process_iterable ===\n";
    process_iterable(vec);

    std::cout << "\n=== smart_process ===\n";
    smart_process(42);
    smart_process(vec);
    smart_process(std::string{"Hi"});

    std::cout << "\n=== process_requires ===\n";
    process_requires(vec);

    std::cout << "\n=== no_deduction ===\n";
    no_deduction<int>(10);

    std::cout << "\n=== check_push_back_support ===\n";
    check_push_back_support<std::vector<int>>();
    check_push_back_support<std::map<int, int>>();

    std::cout << "\n=== add_values ===\n";
    std::cout << "add_values(5,7)=" << add_values(5, 7) << "\n";

    std::cout << "\n=== check_pointer ===\n";
    check_pointer<int>();
    check_pointer<int*>();

    std::cout << "\n=== smart_process (list) ===\n";
    const std::list<int> lst{4, 5, 6};
    smart_process(lst);

    assert(add_values(2, 3) == 5);

    return 0;
}
