#include <iostream>
#include <utility>
#include <type_traits>
#include <algorithm>
#include <tuple>
#include <cassert>
#include <cstddef>

template<typename... Args>
void print_all(const Args&... args) {
    std::size_t i = 0;
    ((std::cout << args << (++i < sizeof...(Args) ? " " : "\n")), ...);
}

template<typename... Args>
    requires (std::is_arithmetic_v<Args> && ...)
[[nodiscard]] constexpr auto sum(Args... args) noexcept {
    return (args + ...);
}

template<typename... Args>
[[nodiscard]] constexpr auto sum_with_init(Args... args) noexcept {
    return (0 + ... + args);
}

template<typename... Args>
[[nodiscard]] constexpr auto subtract_all(Args... args) noexcept {
    return (args - ...);
}

template<typename... Args>
    requires (sizeof...(Args) > 0) && (std::is_arithmetic_v<Args> && ...)
[[nodiscard]] constexpr auto average(Args... args) noexcept {
    return sum(args...) / static_cast<decltype(sum(args...))>(sizeof...(args));
}

template<typename... Args>
    requires (std::is_arithmetic_v<Args> && ...)
[[nodiscard]] constexpr auto multiply(Args... args) noexcept {
    return (args * ...);
}

template<typename... Args>
void print_forward(Args&&... args) {
    (std::cout << ... << args) << "\n";
}

template<typename... Args>
void print_lines(Args&&... args) {
    ((std::cout << args << "\n"), ...);
}

template<typename... Args>
void print_csv(Args&&... args) {
    std::size_t n = 0;
    ((std::cout << args << (++n < sizeof...(Args) ? ", " : "")), ...);
    std::cout << "\n";
}

template<typename... Args>
[[nodiscard]] constexpr std::size_t count_args(Args...) noexcept {
    return sizeof...(Args);
}

template<typename T, typename... Args>
[[nodiscard]] constexpr bool all_same() noexcept {
    return (std::is_same_v<T, Args> && ...);
}

template<typename... Args>
[[nodiscard]] constexpr bool any_floating() noexcept {
    return (std::is_floating_point_v<Args> || ...);
}

template<typename... Args>
[[nodiscard]] constexpr bool logical_and(Args... args) noexcept {
    return (args && ...);
}

template<typename... Args>
[[nodiscard]] constexpr bool logical_or(Args... args) noexcept {
    return (args || ...);
}

template<typename... Args>
    requires (sizeof...(Args) > 0)
[[nodiscard]] auto min_value(Args... args) {
    return std::min({args...});
}

template<typename... Args>
    requires (sizeof...(Args) > 0)
[[nodiscard]] auto max_value(Args... args) {
    return std::max({args...});
}

template<typename Func, typename... Args>
void invoke_all(Func f, Args&&... args) {
    (f(std::forward<Args>(args)), ...);
}

template<typename Pred, typename... Args>
[[nodiscard]] constexpr auto sum_if(Pred p, Args... args) {
    return ((p(args) ? args : decltype(args){}) + ...);
}

template<typename Pred, typename... Args>
[[nodiscard]] constexpr std::size_t count_if_pack(Pred p, Args... args) {
    return ((p(args) ? std::size_t{1} : std::size_t{0}) + ...);
}

template<typename... Args>
void print_pack_info() {
    std::cout << "Pack size=" << sizeof...(Args) << "\n";
}

template<typename... Args>
[[nodiscard]] auto make_tuple_pack(Args&&... args) {
    return std::make_tuple(std::forward<Args>(args)...);
}

static_assert(sum(1, 2, 3)             == 6);
static_assert(multiply(2, 3, 4)        == 24);
static_assert(subtract_all(100, 10, 5) == 85);
static_assert(any_floating<int, double>());
static_assert(!any_floating<int, char>());
static_assert(all_same<int, int, int>());
static_assert(!all_same<int, double>());
static_assert(logical_and(true, true));
static_assert(!logical_and(true, false));
static_assert(logical_or(false, true));
static_assert(count_args(1, 2, 3) == 3);

int main() {
    std::cout << "=== print_all ===\n";
    print_all(1, 2.5, "hello", 3, 4);

    std::cout << "\n=== fold arithmetic ===\n";
    std::cout << "sum(1..5)="        << sum(1, 2, 3, 4, 5)     << "\n"
              << "sum_with_init="    << sum_with_init(10, 20, 30) << "\n"
              << "subtract_all="     << subtract_all(100, 10, 5)  << "\n"
              << "average(2,4,6,8)=" << average(2, 4, 6, 8)      << "\n"
              << "multiply(2,3,4)="  << multiply(2, 3, 4)         << "\n";

    std::cout << "\n=== print_forward ===\n";
    print_forward("Forwarded: ", 42, " ", 3.14);

    std::cout << "\n=== print_lines ===\n";
    print_lines("Line 1", 123, 4.56);

    std::cout << "\n=== print_csv ===\n";
    print_csv(10, 20, 30, 40);

    std::cout << "\n=== pack meta ===\n";
    std::cout << "count_args=" << count_args(1, 2, 3, 4, 5) << "\n"
              << "all_same<int,int,int>=" << std::boolalpha << all_same<int, int, int>() << "\n"
              << "all_same<int,double>="  << all_same<int, double>()                     << "\n"
              << "any_floating<int,char,double>=" << any_floating<int, char, double>()   << "\n";

    std::cout << "\n=== min / max ===\n";
    std::cout << "min(5,2,8,1)=" << min_value(5, 2, 8, 1) << "\n"
              << "max(5,2,8,1)=" << max_value(5, 2, 8, 1) << "\n";

    std::cout << "\n=== logical folds ===\n";
    std::cout << "AND(T,T,F)=" << logical_and(true, true, false) << "\n"
              << "OR(F,F,T)="  << logical_or(false, false, true)  << "\n";

    std::cout << "\n=== invoke_all ===\n";
    invoke_all([](auto x) { std::cout << "Value=" << x << "\n"; }, 1, 2, 3);

    std::cout << "\n=== sum_if (even) ===\n";
    std::cout << "sum_if_even=" << sum_if([](int x){ return x % 2 == 0; }, 1, 2, 3, 4, 5, 6) << "\n";

    std::cout << "\n=== count_if_pack (even) ===\n";
    std::cout << "count_even=" << count_if_pack([](int x){ return x % 2 == 0; }, 1, 2, 3, 4, 5, 6) << "\n";

    std::cout << "\n=== pack info / tuple ===\n";
    print_pack_info<int, double, char>();
    auto tup = make_tuple_pack(1, 2.5, "tuple");
    std::cout << "tuple size=" << std::tuple_size_v<decltype(tup)> << "\n";

    assert(sum(1, 2, 3) == 6);

    return 0;
}
