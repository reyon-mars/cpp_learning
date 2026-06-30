#include <iostream>
#include <type_traits>
#include <vector>
#include <string>
#include <string_view>
#include <iterator>
#include <list>
#include <map>
#include <set>
#include <cassert>
#include <utility>
#include <optional>
#include <algorithm>
#include <ranges>

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

template<typename T>
concept HasFind = requires(T t, typename T::key_type k) {
    { t.find(k) };
};

template<typename T>
concept Hashable = requires(const T& t) {
    { std::hash<T>{}(t) } -> std::convertible_to<std::size_t>;
};

template<typename T>
concept Sortable = Iterable<T> && requires(T t) {
    { std::sort(std::begin(t), std::end(t)) };
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

template<typename T>
void check_find_support() {
    if constexpr (HasFind<T>)
        std::cout << "supports find\n";
    else
        std::cout << "does NOT support find\n";
}

template<typename T>
void check_hashable() {
    if constexpr (Hashable<T>)
        std::cout << "hashable\n";
    else
        std::cout << "not hashable\n";
}

template<HasSize T>
[[nodiscard]] bool is_empty(const T& container) noexcept {
    return container.size() == 0;
}

template<typename T>
[[nodiscard]] auto first_or_nullopt(const T& container) -> std::optional<typename T::value_type> {
    if constexpr (HasSize<T>) {
        if (container.empty()) return std::nullopt;
    }
    return *std::begin(container);
}

template<Arithmetic T>
[[nodiscard]] constexpr T add_values(T a, T b) noexcept { return a + b; }

template<Arithmetic... Ts>
[[nodiscard]] constexpr auto sum_all(Ts... values) noexcept {
    return (values + ...);
}

static_assert(add_values(2, 3) == 5);
static_assert(add_values(1.5, 2.5) == 4.0);
static_assert(sum_all(1, 2, 3, 4) == 10);

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

    std::cout << "\n=== check_find_support ===\n";
    check_find_support<std::map<int, int>>();
    check_find_support<std::vector<int>>();

    std::cout << "\n=== check_hashable ===\n";
    check_hashable<int>();
    check_hashable<std::string>();

    std::cout << "\n=== add_values / sum_all ===\n";
    std::cout << "add_values(5,7)=" << add_values(5, 7) << "\n"
              << "sum_all(1,2,3,4,5)=" << sum_all(1, 2, 3, 4, 5) << "\n";

    std::cout << "\n=== check_pointer ===\n";
    check_pointer<int>();
    check_pointer<int*>();

    std::cout << "\n=== smart_process (list) ===\n";
    const std::list<int> lst{4, 5, 6};
    smart_process(lst);

    std::cout << "\n=== is_empty / first_or_nullopt ===\n";
    const std::vector<int> empty_vec;
    std::cout << "is_empty(vec)="       << std::boolalpha << is_empty(vec)        << "\n"
              << "is_empty(empty_vec)=" << is_empty(empty_vec)                    << "\n";
    if (auto v = first_or_nullopt(vec))       std::cout << "first(vec)="       << *v << "\n";
    if (!first_or_nullopt(empty_vec))          std::cout << "first(empty_vec)=nullopt\n";

    assert(add_values(2, 3) == 5);
    assert(is_empty(empty_vec));
    assert(!is_empty(vec));

    std::cout << "\nAll assertions passed.\n";
    return 0;
}
