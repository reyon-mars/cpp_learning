#include <algorithm>
#include <cctype>
#include <iostream>
#include <numeric>
#include <span>
#include <string>
#include <string_view>
#include <vector>
#include <format>
#include <ranges>
#include <cassert>
#include <optional>
#include <concepts>
#include <array>

namespace utils {

void greet()       { std::cout << "Hello, User!\n"; }
void say_goodbye() { std::cout << "Goodbye! End of program.\n"; }
void print_divider() { std::cout << "-----------------------------\n"; }

void print_message(std::string_view msg)    { std::cout << std::format("{}\n", msg); }
void print_status (std::string_view status) { std::cout << std::format("[STATUS] {}\n", status); }

template <std::ranges::input_range R>
void print_range(const R& r, std::string_view label = "") {
    if (!label.empty()) std::cout << std::format("{}: ", label);
    for (const auto& v : r) std::cout << std::format("{} ", v);
    std::cout << '\n';
}

class Demo {
public:
    void show() const { std::cout << "Demo::show()\n"; }
};

struct Info   { int  value = 10;    };
struct Stats  { int  runs  = 1;     };
struct Config { bool debug = false; };

inline void toggle_debug(Config& cfg) noexcept { cfg.debug = !cfg.debug; }
inline void record_run  (Stats& stats) noexcept { ++stats.runs; }

[[nodiscard]] constexpr int    add     (int a, int b)  noexcept { return a + b; }
[[nodiscard]] constexpr int    multiply(int a, int b)  noexcept { return a * b; }
[[nodiscard]] constexpr int    increment(int x)        noexcept { return x + 1; }
[[nodiscard]] constexpr int    square  (int x)         noexcept { return x * x; }
[[nodiscard]] constexpr int    cube    (int x)         noexcept { return x * x * x; }
[[nodiscard]] constexpr int    absolute(int x)         noexcept { return x < 0 ? -x : x; }
[[nodiscard]] constexpr double average (int a, int b)  noexcept { return (a + b) / 2.0; }
[[nodiscard]] constexpr int    clamp   (int v, int lo, int hi) noexcept {
    return v < lo ? lo : v > hi ? hi : v;
}

[[nodiscard]] constexpr int factorial(int n) noexcept {
    return (n <= 1) ? 1 : n * factorial(n - 1);
}

[[nodiscard]] constexpr long long fibonacci(int n) noexcept {
    if (n <= 1) return n;
    long long a = 0, b = 1;
    for (int i = 2; i <= n; ++i) { long long t = a + b; a = b; b = t; }
    return b;
}

[[nodiscard]] constexpr int gcd(int a, int b) noexcept {
    return b == 0 ? absolute(a) : gcd(b, a % b);
}

[[nodiscard]] constexpr int lcm(int a, int b) noexcept {
    return (a / gcd(a, b)) * b;
}

[[nodiscard]] constexpr bool is_positive    (int x) noexcept { return x > 0; }
[[nodiscard]] constexpr bool is_even        (int x) noexcept { return x % 2 == 0; }
[[nodiscard]] constexpr bool is_odd         (int x) noexcept { return x % 2 != 0; }
[[nodiscard]] constexpr bool is_zero        (int x) noexcept { return x == 0; }
[[nodiscard]] constexpr bool is_valid_number(int x) noexcept { return x >= 0 && x <= 100; }
[[nodiscard]] constexpr bool is_prime       (int x) noexcept {
    if (x < 2) return false;
    for (int i = 2; i * i <= x; ++i)
        if (x % i == 0) return false;
    return true;
}

[[nodiscard]] bool      is_empty  (std::string_view s) noexcept { return s.empty(); }
[[nodiscard]] std::size_t char_count(std::string_view s) noexcept { return s.size(); }

template <typename T>
[[nodiscard]] constexpr T max_value(T a, T b) noexcept { return a > b ? a : b; }

template <typename T>
[[nodiscard]] constexpr T min_value(T a, T b) noexcept { return a < b ? a : b; }

[[nodiscard]] int sum_vector(std::span<const int> v) noexcept {
    return std::reduce(v.begin(), v.end(), 0);
}

[[nodiscard]] std::optional<int> max_in_vector(std::span<const int> v) {
    if (v.empty()) return std::nullopt;
    return *std::ranges::max_element(v);
}

[[nodiscard]] std::optional<int> min_in_vector(std::span<const int> v) {
    if (v.empty()) return std::nullopt;
    return *std::ranges::min_element(v);
}

[[nodiscard]] std::optional<double> average_vector(std::span<const int> v) {
    if (v.empty()) return std::nullopt;
    return static_cast<double>(sum_vector(v)) / static_cast<double>(v.size());
}

[[nodiscard]] std::size_t vector_size(std::span<const int> v) noexcept { return v.size(); }

[[nodiscard]] std::string to_upper(std::string str) {
    std::ranges::transform(str, str.begin(),
                           [](unsigned char c) { return static_cast<char>(std::toupper(c)); });
    return str;
}

[[nodiscard]] std::string to_lower(std::string str) {
    std::ranges::transform(str, str.begin(),
                           [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return str;
}

[[nodiscard]] std::string reverse_string(std::string str) {
    std::ranges::reverse(str);
    return str;
}

[[nodiscard]] bool starts_with_util(std::string_view s, std::string_view prefix) noexcept {
    return s.starts_with(prefix);
}

[[nodiscard]] bool ends_with_util(std::string_view s, std::string_view suffix) noexcept {
    return s.ends_with(suffix);
}

template <std::invocable<int> Fn>
[[nodiscard]] std::vector<int> transform_vec(std::span<const int> v, Fn fn) {
    std::vector<int> out(v.size());
    std::ranges::transform(v, out.begin(), fn);
    return out;
}

template <std::predicate<int> Pred>
[[nodiscard]] std::vector<int> filter_vec(std::span<const int> v, Pred p) {
    std::vector<int> out;
    std::ranges::copy_if(v, std::back_inserter(out), p);
    return out;
}

} // namespace utils

int main() {
    using namespace utils;

    std::cout << "Hello, World\n";
    greet();
    std::cout << std::format("5 + 7 = {}\n", add(5, 7));

    Demo demo;
    demo.show();

    Info info;
    std::cout << std::format("Info value = {}\n", info.value);

    print_message("This is a simple helper function");

    int execution_count = increment(1);
    std::cout << std::format("Execution count: {}\n", execution_count);
    std::cout << std::format("Is positive? {}\nIs even?     {}\n",
                             is_positive(execution_count) ? "Yes" : "No",
                             is_even(execution_count)     ? "Yes" : "No");

    print_divider();

    Stats stats;
    record_run(stats);
    std::cout << std::format("Total runs: {}\n", stats.runs);
    std::cout << std::format("3 * 4 = {}\n", multiply(3, 4));

    print_status("Program running normally");

    Config cfg;
    toggle_debug(cfg);
    std::cout << std::format("Debug mode: {}\n", cfg.debug ? "ON" : "OFF");

    std::cout << std::format("square(5)        = {}\n", square(5));
    std::cout << std::format("is_zero(2)?        {}\n", is_zero(execution_count) ? "Yes" : "No");
    std::cout << std::format("absolute(-7)     = {}\n", absolute(-7));
    std::cout << std::format("max(10,20)       = {}\n", max_value(10, 20));
    std::cout << std::format("factorial(5)     = {}\n", factorial(5));
    std::cout << std::format("to_upper:          {}\n", to_upper("hello world"));
    std::cout << std::format("is_valid(50)?      {}\n", is_valid_number(50) ? "Yes" : "No");
    std::cout << std::format("clamp(150,0,100) = {}\n", clamp(150, 0, 100));
    std::cout << std::format("average(10,20)   = {}\n", average(10, 20));
    std::cout << std::format("is_empty(\"\")?   {}\n",  is_empty("") ? "Yes" : "No");

    print_divider();

    std::cout << std::format("min(10,20)       = {}\n", min_value(10, 20));
    std::cout << std::format("cube(3)          = {}\n", cube(3));
    std::cout << std::format("reverse(OpenAI)  = {}\n", reverse_string("OpenAI"));
    std::cout << std::format("fibonacci(10)    = {}\n", fibonacci(10));
    std::cout << std::format("gcd(12,8)        = {}\n", gcd(12, 8));
    std::cout << std::format("lcm(4,6)         = {}\n", lcm(4, 6));
    std::cout << std::format("is_prime(17)?      {}\n", is_prime(17) ? "Yes" : "No");
    std::cout << std::format("is_prime(18)?      {}\n", is_prime(18) ? "Yes" : "No");
    std::cout << std::format("starts_with(hello, he)? {}\n",
                             starts_with_util("hello", "he") ? "Yes" : "No");
    std::cout << std::format("ends_with(world, ld)?   {}\n",
                             ends_with_util("world", "ld") ? "Yes" : "No");

    const std::vector<int> numbers{1, 2, 3, 4, 5};
    print_range(numbers, "Vector");
    std::cout << std::format("sum  = {}\n", sum_vector(numbers));
    if (auto m = max_in_vector(numbers))  std::cout << std::format("max  = {}\n", *m);
    if (auto m = min_in_vector(numbers))  std::cout << std::format("min  = {}\n", *m);
    if (auto a = average_vector(numbers)) std::cout << std::format("avg  = {:.1f}\n", *a);
    std::cout << std::format("size = {}\n", vector_size(numbers));
    std::cout << std::format("char_count(\"ChatGPT\") = {}\n", char_count("ChatGPT"));

    print_divider();

    std::cout << std::format("is_odd(2)?    {}\n", is_odd(execution_count) ? "Yes" : "No");
    std::cout << std::format("to_lower:     {}\n", to_lower("HELLO WORLD"));

    std::cout << "\n=== transform_vec / filter_vec ===\n";
    const auto squares  = transform_vec(numbers, square);
    const auto evens    = filter_vec(numbers, is_even);
    print_range(squares, "squares");
    print_range(evens,   "evens");

    std::cout << "\n=== views pipeline ===\n";
    auto pipeline = numbers
        | std::views::filter(is_odd)
        | std::views::transform(square);
    print_range(pipeline, "odd squares");
    std::cout << std::format("sum of odd squares: {}\n",
                             std::reduce(std::ranges::begin(pipeline),
                                         std::ranges::end(pipeline), 0));

    std::cout << "\n=== Fibonacci sequence ===\n";
    print_range(std::views::iota(0, 10) | std::views::transform(fibonacci), "fib[0..9]");

    std::cout << "\n=== Primes up to 30 ===\n";
    print_range(std::views::iota(2, 31) | std::views::filter(is_prime), "primes");

    static_assert(square(5)         == 25);
    static_assert(cube(3)           == 27);
    static_assert(factorial(5)      == 120);
    static_assert(absolute(-7)      == 7);
    static_assert(clamp(150, 0,100) == 100);
    static_assert(add(5, 7)         == 12);
    static_assert(fibonacci(10)     == 55);
    static_assert(gcd(12, 8)        == 4);
    static_assert(lcm(4,  6)        == 12);
    static_assert(is_prime(17));
    static_assert(!is_prime(18));
    static_assert(is_even(4));
    static_assert(is_odd(3));
    static_assert(is_zero(0));
    static_assert(is_valid_number(50));
    static_assert(!is_valid_number(200));

    std::cout << "\nAll static assertions passed.\n";
    print_divider();
    std::cout << "Program finished successfully.\n";
    say_goodbye();

    return 0;
}
