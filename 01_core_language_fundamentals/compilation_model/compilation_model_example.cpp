#include <iostream>
#include <limits>
#include <format>
#include <string_view>
#include <array>
#include <bit>
#include <ranges>
#include <cassert>
#include <source_location>
#include <type_traits>

#define APP_VERSION "1.0"
#define APP_NAME    "CompilationDemo"
#define MAX_USERS   100

#ifdef NDEBUG
#define DEBUG_BUILD false
#else
#define DEBUG_BUILD true
#endif

#define DOUBLE_MACRO(x) x + x

namespace compiler_demo {

[[nodiscard]] constexpr int  square     (int x) noexcept { return x * x;     }
[[nodiscard]] constexpr int  cube       (int x) noexcept { return x * x * x; }
[[nodiscard]] constexpr int  double_safe(int x) noexcept { return x + x;     }
[[nodiscard]] constexpr int  factorial  (int n) noexcept { return n <= 1 ? 1 : n * factorial(n - 1); }
[[nodiscard]] constexpr bool is_power_of_two(unsigned int n) noexcept { return n != 0 && (n & (n - 1)) == 0; }

[[nodiscard]] constexpr int fibonacci(int n) noexcept {
    if (n <= 1) return n;
    int a = 0, b = 1;
    for (int i = 2; i <= n; ++i) { int t = a + b; a = b; b = t; }
    return b;
}

static_assert(square(5)          == 25);
static_assert(cube(2)            == 8);
static_assert(double_safe(7)     == 14);
static_assert(factorial(5)       == 120);
static_assert(fibonacci(10)      == 55);
static_assert(is_power_of_two(8));
static_assert(!is_power_of_two(7));

inline int build_number     = 1;
inline int internal_counter = 0;

void print_version()   { std::cout << std::format("Version: {}\n", APP_VERSION); }
void print_app_name()  { std::cout << std::format("App:     {}\n", APP_NAME);    }
void print_divider()   { std::cout << "-----------------------------\n"; }

void print_build_info() {
    std::cout << std::format("Build date: {}\nBuild time: {}\n", __DATE__, __TIME__);
}

void print_compiler_info() {
#if defined(__clang__)
    std::cout << "Compiler: Clang\n";
#elif defined(__GNUC__)
    std::cout << "Compiler: GCC\n";
#elif defined(_MSC_VER)
    std::cout << "Compiler: MSVC\n";
#else
    std::cout << "Compiler: Unknown\n";
#endif
}

void debug_message() {
    if constexpr (DEBUG_BUILD)
        std::cout << "[DEBUG] Debug build active\n";
}

void external_function() { std::cout << "Simulated external function call\n"; }

namespace {
    void internal_helper() { std::cout << "Internal (anonymous-namespace) helper\n"; }
}

void print_binary(unsigned int value) {
    constexpr int bits = std::numeric_limits<unsigned int>::digits;
    std::cout << std::format("Binary ({:>3}): ", value);
    for (int i = bits - 1; i >= 0; --i)
        std::cout << ((value >> i) & 1u);
    std::cout << std::format("  (popcount={})\n", std::popcount(value));
}

void show_compilation_steps() {
    constexpr std::array steps{
        "1. Preprocessing",
        "2. Parsing",
        "3. Compilation",
        "4. Linking",
    };
    for (std::string_view step : steps)
        std::cout << std::format("  {}\n", step);
}

void log_with_location(std::string_view msg,
                        std::source_location loc = std::source_location::current()) {
    std::cout << std::format("[{}:{}] {}\n", loc.file_name(), loc.line(), msg);
}

template <typename T>
void print_type_traits() {
    std::cout << std::format("Type traits for {}:\n"
                             "  is_integral:  {}\n"
                             "  is_signed:    {}\n"
                             "  sizeof:       {}\n",
                             typeid(T).name(),
                             std::is_integral_v<T>,
                             std::is_signed_v<T>,
                             sizeof(T));
}

constexpr auto generate_squares(int n) {
    std::array<int, 10> arr{};
    for (int i = 0; i < n && i < 10; ++i)
        arr[i] = square(i + 1);
    return arr;
}

} // namespace compiler_demo

int main() {
    using namespace compiler_demo;

    std::cout << "Compiled successfully\n";
    print_divider();
    print_app_name();
    print_version();
    std::cout << std::format("Build number: {}\n", build_number);
    print_divider();
    print_build_info();
    print_compiler_info();
    print_divider();
    debug_message();
    ++internal_counter;
    std::cout << std::format("Internal counter: {}\n", internal_counter);
    print_divider();

    std::cout << std::format("square(5)         = {}\n", square(5));
    std::cout << std::format("cube(3)           = {}\n", cube(3));
    std::cout << std::format("DOUBLE_MACRO(5*2) = {}  (macro pitfall: expands to 5*2+5*2)\n",
                             DOUBLE_MACRO(5 * 2));
    std::cout << std::format("double_safe(5*2)  = {}\n", double_safe(5 * 2));
    std::cout << std::format("factorial(6)      = {}\n", factorial(6));
    std::cout << std::format("fibonacci(10)     = {}\n", fibonacci(10));
    print_divider();

    std::cout << std::format("MAX_USERS = {}\n", MAX_USERS);
    external_function();
    internal_helper();
    print_divider();

    constexpr int val      = square(6);
    constexpr int cube_val = cube(4);
    constexpr int fib_val  = fibonacci(12);
    std::cout << std::format("Compile-time square(6)   = {}\n", val);
    std::cout << std::format("Compile-time cube(4)     = {}\n", cube_val);
    std::cout << std::format("Compile-time fib(12)     = {}\n", fib_val);
    print_divider();

    std::cout << "Compilation stages:\n";
    show_compilation_steps();
    print_divider();

    std::cout << "Binary representations:\n";
    for (unsigned int v : {42u, 255u, 128u, 0u})
        print_binary(v);
    print_divider();

    std::cout << "is_power_of_two: ";
    for (int v : std::views::iota(0, 9))
        std::cout << std::format("{}:{} ", v, is_power_of_two(static_cast<unsigned>(v)) ? "Y" : "N");
    std::cout << '\n';
    print_divider();

    std::cout << "source_location demo:\n";
    log_with_location("reached main checkpoint");
    print_divider();

    std::cout << "Type traits:\n";
    print_type_traits<int>();
    print_type_traits<unsigned long>();
    print_divider();

    constexpr auto sq_table = generate_squares(5);
    std::cout << "Compile-time squares [1..5]: ";
    for (int i = 0; i < 5; ++i) std::cout << std::format("{} ", sq_table[i]);
    std::cout << '\n';
    print_divider();

    std::cout << "Compilation Model Summary:\n"
              << "  Macros     -> expanded during preprocessing\n"
              << "  Syntax     -> checked during parsing\n"
              << "  Code       -> translated during compilation\n"
              << "  Symbols    -> resolved during linking\n";
    print_divider();

    static_assert(generate_squares(5)[0] == 1);
    static_assert(generate_squares(5)[4] == 25);
    static_assert(std::popcount(0b1010'1010u) == 4);

    std::cout << "All static assertions passed.\n";
    return 0;
}
