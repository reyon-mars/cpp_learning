#include <cassert>
#include <iostream>
#include <string>
#include <string_view>
#include <type_traits>
#include <typeinfo>
#include <vector>
#include <functional>
#include <optional>
#include <utility>
#include <concepts>
#include <algorithm>
#include <numeric>

template<typename T>
void print_type() {
    std::cout << "type=" << typeid(T).name() << "\n";
}

template<typename T, typename U>
[[nodiscard]] auto multiply_generic(T a, U b) -> decltype(a * b) {
    return a * b;
}

template<typename T, typename U>
[[nodiscard]] auto add_generic(T a, U b) {
    return a + b;
}

template<typename T>
void forward_test(T&& param) {
    if constexpr (std::is_lvalue_reference_v<T>)
        std::cout << "Lvalue\n";
    else
        std::cout << "Rvalue\n";
}

[[nodiscard]] decltype(auto) return_value() {
    int x = 10;
    return x;
}

[[nodiscard]] decltype(auto) return_reference() {
    static int x = 50;
    return (x);
}

template<typename T>
void check_integral() {
    std::cout << "is_integral=" << std::boolalpha << std::is_integral_v<T> << "\n";
}

template<typename T>
void universal_ref_demo(T&& value) {
    std::cout << "universal_ref value=" << value << "\n";
}

[[nodiscard]] constexpr auto square_auto(auto x) noexcept { return x * x; }

template<typename T>
void type_decay_demo(T) {
    std::cout << "decayed type=" << typeid(T).name() << "\n";
}

template<typename T>
void print_vector_info(const std::vector<T>& vec) {
    std::cout << "vector: size=" << vec.size()
              << " elem_type=" << typeid(T).name() << "\n";
}

template<typename T, typename U>
[[nodiscard]] auto common_type_demo(T a, U b) {
    using C = std::common_type_t<T, U>;
    std::cout << "common_type: " << typeid(C).name() << "\n";
    return static_cast<C>(a) + static_cast<C>(b);
}

template<bool Condition, typename T, typename F>
using select_t = std::conditional_t<Condition, T, F>;

template<typename T>
void conditional_type_demo() {
    using Selected = select_t<std::is_integral_v<T>, long long, double>;
    std::cout << "conditional selected: " << typeid(Selected).name() << "\n";
}

template<std::invocable<int> Fn>
[[nodiscard]] auto invoke_with_10(Fn&& fn) {
    return std::invoke(std::forward<Fn>(fn), 10);
}

template<typename T>
[[nodiscard]] auto make_optional_if(bool cond, T val) -> std::optional<T> {
    return cond ? std::optional<T>{val} : std::nullopt;
}

[[nodiscard]] auto make_adder(int n) {
    return [n](auto x) noexcept { return x + n; };
}

[[nodiscard]] auto make_multiplier(auto factor) noexcept {
    return [factor](auto x) noexcept { return x * factor; };
}

template<typename T>
[[nodiscard]] decltype(auto) identity(T&& v) noexcept {
    return std::forward<T>(v);
}

template<typename Container>
void auto_range_demo(const Container& c) {
    std::cout << "auto range: ";
    for (const auto& v : c) std::cout << v << " ";
    std::cout << "\n";
}

template<typename T>
[[nodiscard]] constexpr T clamp_auto(T v, T lo, T hi) noexcept {
    return v < lo ? lo : v > hi ? hi : v;
}

[[nodiscard]] auto tuple_first(auto&& tup) -> decltype(auto) {
    return std::get<0>(std::forward<decltype(tup)>(tup));
}

static_assert(square_auto(6)   == 36);
static_assert(square_auto(2.0) == 4.0);
static_assert(clamp_auto(15, 0, 10) == 10);
static_assert(clamp_auto(-5, 0, 10) == 0);
static_assert(std::is_same_v<decltype(add_generic(1, 2.0)), double>);

int main() {
    std::cout << "=== initializer_list deduction ===\n";
    auto list = {1, 2, 3};
    print_type<decltype(list)>();

    std::cout << "\n=== const stripping by auto ===\n";
    const int ci = 10;
    auto       copy  = ci;
    const auto copy2 = ci;
    std::cout << std::boolalpha
              << "copy is_const="  << std::is_const_v<decltype(copy)>  << "\n"
              << "copy2 is_const=" << std::is_const_v<decltype(copy2)> << "\n";

    std::cout << "\n=== trailing return type ===\n";
    auto mul = multiply_generic(2, 3.5);
    static_assert(std::is_same_v<decltype(mul), double>);
    std::cout << "multiply(2, 3.5)=" << mul << "\n";

    std::cout << "\n=== perfect forwarding ===\n";
    int a = 5;
    forward_test(a);
    forward_test(10);

    std::cout << "\n=== decltype(auto) value vs reference ===\n";
    auto val = return_value();
    std::cout << "return_value()=" << val << "\n";
    decltype(auto) ref = return_reference();
    assert(ref == 50);
    ref = 100;
    std::cout << "After ref=100, return_reference()=" << return_reference() << "\n";

    std::cout << "\n=== add_generic ===\n";
    auto sum = add_generic(10, 2.5);
    std::cout << "add(10, 2.5)=" << sum << "\n";

    std::cout << "\n=== check_integral ===\n";
    check_integral<int>();
    check_integral<double>();

    std::cout << "\n=== universal reference ===\n";
    universal_ref_demo(42);
    std::string text = "Hello";
    universal_ref_demo(text);

    std::cout << "\n=== constexpr auto (square_auto) ===\n";
    constexpr auto sq = square_auto(6);
    std::cout << "square_auto(6)=" << sq << "\n";

    std::cout << "\n=== type decay ===\n";
    const int value = 99;
    type_decay_demo(value);

    std::cout << "\n=== vector type info ===\n";
    const std::vector<std::string> names{"Alice", "Bob", "Charlie"};
    print_vector_info(names);

    std::cout << "\n=== decltype variable ===\n";
    decltype(sum) another = 55.5;
    std::cout << "decltype(sum) another=" << another << "\n";

    std::cout << "\n=== std::common_type ===\n";
    auto ct1 = common_type_demo(3, 4.5);
    auto ct2 = common_type_demo(10, 20);
    std::cout << "common(3, 4.5)="  << ct1 << "\n"
              << "common(10, 20)="  << ct2 << "\n";

    std::cout << "\n=== std::conditional ===\n";
    conditional_type_demo<int>();
    conditional_type_demo<double>();

    std::cout << "\n=== std::invoke ===\n";
    std::cout << "invoke_with_10(square)=" << invoke_with_10([](int x){ return x * x; }) << "\n";
    std::cout << "invoke_with_10(+5)="     << invoke_with_10([](int x){ return x + 5;  }) << "\n";

    std::cout << "\n=== make_adder / make_multiplier ===\n";
    auto add5    = make_adder(5);
    auto triple  = make_multiplier(3);
    std::cout << "add5(10)="   << add5(10)   << "\n"
              << "triple(7)="  << triple(7)  << "\n"
              << "triple(2.5)="<< triple(2.5)<< "\n";

    std::cout << "\n=== identity (perfect forward) ===\n";
    int x = 77;
    decltype(auto) id_ref = identity(x);
    std::cout << "identity(x)=" << id_ref << "\n";
    id_ref = 88;
    std::cout << "x after id_ref=88: " << x << "\n";

    std::cout << "\n=== auto lambda parameter ===\n";
    auto printer = [](const auto& v) { std::cout << "auto lambda: " << v << "\n"; };
    printer(42);
    printer(3.14);
    printer(std::string_view{"C++20"});

    std::cout << "\n=== optional deduction ===\n";
    auto opt1 = make_optional_if(true,  100);
    auto opt2 = make_optional_if(false, 200);
    std::cout << "opt1=" << opt1.value_or(-1) << "\n"
              << "opt2=" << opt2.value_or(-1) << "\n";

    std::cout << "\n=== auto range ===\n";
    auto_range_demo(names);
    const std::vector<int> ints{1, 2, 3, 4, 5};
    auto_range_demo(ints);

    std::cout << "\n=== clamp_auto ===\n";
    std::cout << "clamp(15,0,10)="  << clamp_auto(15, 0, 10)  << "\n"
              << "clamp(-5,0,10)="  << clamp_auto(-5, 0, 10)  << "\n"
              << "clamp(5,0,10)="   << clamp_auto(5,  0, 10)  << "\n";

    std::cout << "\n=== tuple_first ===\n";
    auto tup = std::make_tuple(42, "hello", 3.14);
    std::cout << "tuple_first=" << tuple_first(tup) << "\n";

    assert(add5(10)                        == 15);
    assert(triple(4)                       == 12);
    assert(invoke_with_10([](int x){ return x * x; }) == 100);
    assert(clamp_auto(15, 0, 10)           == 10);
    assert(make_optional_if(true, 7).value_or(-1) == 7);

    std::cout << "\nCompleted.\n";
    return 0;
}
