#include <iostream>
#include <concepts>
#include <vector>
#include <list>
#include <array>
#include <string>
#include <string_view>
#include <type_traits>
#include <algorithm>
#include <numeric>
#include <ranges>
#include <span>
#include <optional>
#include <functional>
#include <cassert>
#include <utility>
#include <iterator>

template<typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;

template<typename T>
concept Container = requires(T t) {
    { t.begin()  };
    { t.end()    };
    { t.size()   } -> std::convertible_to<std::size_t>;
    typename T::value_type;
};

template<typename T>
concept Printable = requires(T t) {
    { std::cout << t } -> std::same_as<std::ostream&>;
};

template<typename T>
concept Comparable = requires(const T& a, const T& b) {
    { a == b } -> std::convertible_to<bool>;
    { a <  b } -> std::convertible_to<bool>;
};

template<typename T>
concept Sortable = Container<T> && Comparable<typename T::value_type>;

template<typename F, typename T>
concept UnaryPredicate = requires(F f, T t) {
    { f(t) } -> std::convertible_to<bool>;
};

template<typename F, typename T>
concept Transformer = requires(F f, T t) {
    { f(t) } -> std::same_as<T>;
};

template<Numeric T>
[[nodiscard]] constexpr T add(T a, T b)      noexcept { return a + b; }

template<Numeric T>
[[nodiscard]] constexpr T subtract(T a, T b) noexcept { return a - b; }

template<Numeric T>
[[nodiscard]] constexpr T multiply(T a, T b) noexcept { return a * b; }

template<Numeric T>
[[nodiscard]] constexpr T divide(T a, T b)            { return a / b; }

template<Numeric T>
constexpr void increment(T& value) noexcept { ++value; }

template<Numeric T>
[[nodiscard]] constexpr T clamp(T value, T lo, T hi) noexcept {
    return value < lo ? lo : value > hi ? hi : value;
}

template<Numeric T>
[[nodiscard]] constexpr T power(T base, int exp) noexcept {
    T result{1};
    for (int i = 0; i < exp; ++i) result *= base;
    return result;
}

template<Numeric T>
class Calculator {
public:
    [[nodiscard]] constexpr T square(T v)         const noexcept { return v * v;             }
    [[nodiscard]] constexpr T cube(T v)           const noexcept { return v * v * v;         }
    [[nodiscard]] constexpr T absolute(T v)       const noexcept { return v < T{} ? -v : v;  }
    [[nodiscard]] constexpr bool is_positive(T v) const noexcept { return v > T{};            }
    [[nodiscard]] constexpr bool is_zero(T v)     const noexcept { return v == T{};           }
    [[nodiscard]] constexpr T negate(T v)         const noexcept { return -v;                 }
};

template<Printable T>
void print_value(const T& value) {
    std::cout << "Printable: " << value << "\n";
}

template<Container C>
void print_container(const C& cont) {
    std::cout << "size=" << cont.size() << "\n";
}

template<Container C>
void print_elements(const C& cont) {
    for (const auto& v : cont) std::cout << v << " ";
    std::cout << "\n";
}

template<Container C>
[[nodiscard]] auto sum_container(const C& cont) {
    using T = typename C::value_type;
    return std::accumulate(cont.begin(), cont.end(), T{});
}

template<Container C>
[[nodiscard]] double average_container(const C& cont) {
    return static_cast<double>(sum_container(cont))
         / static_cast<double>(cont.size());
}

template<Container C>
[[nodiscard]] auto max_element(const C& cont) -> std::optional<typename C::value_type> {
    if (cont.empty()) return std::nullopt;
    return *std::ranges::max_element(cont);
}

template<Container C>
[[nodiscard]] auto min_element(const C& cont) -> std::optional<typename C::value_type> {
    if (cont.empty()) return std::nullopt;
    return *std::ranges::min_element(cont);
}

template<Sortable C>
[[nodiscard]] C sorted_copy(C cont) {
    std::ranges::sort(cont);
    return cont;
}

template<Container C, UnaryPredicate<typename C::value_type> Pred>
[[nodiscard]] std::vector<typename C::value_type> filter_container(const C& cont, Pred pred) {
    std::vector<typename C::value_type> result;
    std::ranges::copy_if(cont, std::back_inserter(result), pred);
    return result;
}

template<Container C, Transformer<typename C::value_type> F>
[[nodiscard]] std::vector<typename C::value_type> transform_container(const C& cont, F fn) {
    std::vector<typename C::value_type> result(cont.size());
    std::ranges::transform(cont, result.begin(), fn);
    return result;
}

template<Comparable T>
[[nodiscard]] bool are_equal(const T& a, const T& b) { return a == b; }

template<Comparable T>
[[nodiscard]] const T& clamp_ref(const T& value, const T& lo, const T& hi) {
    return value < lo ? lo : hi < value ? hi : value;
}

int main() {
    static_assert(Numeric<int>);
    static_assert(Numeric<double>);
    static_assert(!Numeric<std::string>);
    static_assert(Printable<int>);
    static_assert(Container<std::vector<int>>);
    static_assert(Comparable<int>);
    static_assert(Comparable<std::string>);
    static_assert(Sortable<std::vector<int>>);

    std::cout << "=== Arithmetic ===\n";
    std::cout << "add(5,3)="         << add(5, 3)          << "\n"
              << "add(1.5,2.5)="     << add(1.5, 2.5)      << "\n"
              << "multiply(4,6)="    << multiply(4, 6)      << "\n"
              << "subtract(10,3)="   << subtract(10, 3)     << "\n"
              << "divide(20.0,4.0)=" << divide(20.0, 4.0)  << "\n"
              << "clamp(15,0,10)="   << clamp(15, 0, 10)   << "\n"
              << "power(2,8)="       << power(2, 8)         << "\n";

    std::cout << "\n=== Calculator ===\n";
    constexpr Calculator<int> calc;
    std::cout << "square(7)="      << calc.square(7)                << "\n"
              << "cube(3)="        << calc.cube(3)                  << "\n"
              << "absolute(-9)="   << calc.absolute(-9)             << "\n"
              << "negate(5)="      << calc.negate(5)                << "\n"
              << "is_positive(7)=" << std::boolalpha << calc.is_positive(7) << "\n"
              << "is_zero(0)="     << calc.is_zero(0)               << "\n";

    std::cout << "\n=== Container ===\n";
    std::vector<int>   vec{1, 2, 3};
    std::list<double>  lst{1.1, 2.2, 3.3};
    std::array<int, 4> arr{4, 5, 6, 7};

    print_container(vec);
    print_container(lst);
    print_container(arr);

    std::cout << "sum vec="  << sum_container(vec) << "\n"
              << "sum lst="  << sum_container(lst) << "\n"
              << "sum arr="  << sum_container(arr) << "\n"
              << "avg vec="  << average_container(vec) << "\n"
              << "avg arr="  << average_container(arr) << "\n";

    assert(sum_container(vec) == 6);

    std::cout << "\n=== min/max ===\n";
    if (auto v = max_element(vec)) std::cout << "max vec=" << *v << "\n";
    if (auto v = min_element(vec)) std::cout << "min vec=" << *v << "\n";
    if (auto v = max_element(arr)) std::cout << "max arr=" << *v << "\n";

    std::cout << "\n=== print_elements ===\n";
    print_elements(vec);
    print_elements(arr);

    std::cout << "\n=== sorted_copy ===\n";
    const std::vector<int> unsorted{5, 1, 4, 2, 3};
    print_elements(sorted_copy(unsorted));
    print_elements(unsorted);

    std::cout << "\n=== filter_container ===\n";
    auto evens = filter_container(arr, [](int n) { return n % 2 == 0; });
    print_elements(evens);

    std::cout << "\n=== transform_container ===\n";
    auto doubled = transform_container(vec, [](int n) { return n * 2; });
    print_elements(doubled);

    std::cout << "\n=== Printable ===\n";
    print_value(123);
    print_value(3.14);
    print_value(std::string_view{"concepts"});

    std::cout << "\n=== Comparable ===\n";
    std::cout << "are_equal(10,10)="
              << are_equal(10, 10) << "\n"
              << "are_equal(\"Hello\",\"World\")="
              << are_equal(std::string{"Hello"}, std::string{"World"}) << "\n"
              << "clamp_ref(15,0,10)="
              << clamp_ref(15, 0, 10) << "\n";

    std::cout << "\n=== increment ===\n";
    int val = 5;
    increment(val);
    std::cout << "incremented=" << val << "\n";

    std::cout << "\n=== Constrained lambda ===\n";
    auto printer = []<Printable T>(const T& v) {
        std::cout << "Lambda: " << v << "\n";
    };
    printer(999);
    printer(std::string{"Concepts"});

    auto numeric_square = []<Numeric T>(T v) -> T { return v * v; };
    std::cout << "numeric_square(4)="   << numeric_square(4)   << "\n"
              << "numeric_square(3.0)=" << numeric_square(3.0) << "\n";

    std::cout << "\n=== Sort descending ===\n";
    std::ranges::sort(vec, std::greater<>{});
    print_elements(vec);

    std::cout << "\n=== Ranges pipeline ===\n";
    const std::vector<int> nums{1, 2, 3, 4, 5, 6, 7, 8};
    auto pipeline = nums
        | std::views::filter([](int n) { return n % 2 == 0; })
        | std::views::transform([](int n) { return n * n; });
    for (int n : pipeline) std::cout << n << " ";
    std::cout << "\n";

    return 0;
}
