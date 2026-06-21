#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ranges>
#include <span>
#include <cctype>

namespace utils {

void greet() { std::cout << "Hello, User!\n"; }

void say_goodbye() { std::cout << "Goodbye! End of program.\n"; }

void print_divider() { std::cout << "-----------------------------\n"; }

void print_message(std::string_view msg) { std::cout << msg << '\n'; }

void print_status(std::string_view status) { std::cout << "[STATUS] " << status << '\n'; }

void print_vector(std::span<const int> values) {
    for (int v : values) std::cout << v << ' ';
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
inline void record_run(Stats& stats)  noexcept { ++stats.runs; }

[[nodiscard]] constexpr int  add(int a, int b)        noexcept { return a + b;           }
[[nodiscard]] constexpr int  multiply(int a, int b)   noexcept { return a * b;           }
[[nodiscard]] constexpr int  increment(int x)         noexcept { return x + 1;           }
[[nodiscard]] constexpr int  square(int x)            noexcept { return x * x;           }
[[nodiscard]] constexpr int  cube(int x)              noexcept { return x * x * x;       }
[[nodiscard]] constexpr int  absolute(int x)          noexcept { return x < 0 ? -x : x; }
[[nodiscard]] constexpr double average(int a, int b)  noexcept { return (a + b) / 2.0;  }

[[nodiscard]] constexpr int factorial(int n) noexcept {
    return (n <= 1) ? 1 : n * factorial(n - 1);
}

[[nodiscard]] constexpr int clamp(int value, int lo, int hi) noexcept {
    return value < lo ? lo : value > hi ? hi : value;
}

[[nodiscard]] constexpr bool is_positive(int x)     noexcept { return x > 0;         }
[[nodiscard]] constexpr bool is_even(int x)         noexcept { return x % 2 == 0;    }
[[nodiscard]] constexpr bool is_odd(int x)          noexcept { return x % 2 != 0;    }
[[nodiscard]] constexpr bool is_zero(int x)         noexcept { return x == 0;        }
[[nodiscard]] constexpr bool is_valid_number(int x) noexcept { return x >= 0 && x <= 100; }

[[nodiscard]] bool is_empty(std::string_view str) noexcept { return str.empty(); }

[[nodiscard]] std::size_t char_count(std::string_view str) noexcept { return str.size(); }

template<typename T>
[[nodiscard]] constexpr T max_value(T a, T b) noexcept { return a > b ? a : b; }

template<typename T>
[[nodiscard]] constexpr T min_value(T a, T b) noexcept { return a < b ? a : b; }

[[nodiscard]] int sum_vector(std::span<const int> values) noexcept {
    return std::accumulate(values.begin(), values.end(), 0);
}

[[nodiscard]] int max_in_vector(std::span<const int> values) {
    return *std::ranges::max_element(values);
}

[[nodiscard]] std::size_t vector_size(std::span<const int> values) noexcept {
    return values.size();
}

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

} // namespace utils

int main() {
    using namespace utils;

    std::cout << "Hello, World\n";
    greet();
    std::cout << "5 + 7 = " << add(5, 7) << '\n';

    Demo demo;
    demo.show();

    Info info;
    std::cout << "Info value = " << info.value << '\n';

    print_message("This is a simple helper function");

    int execution_count = increment(1);
    std::cout << "Execution count: " << execution_count << '\n';
    std::cout << "Is positive? " << (is_positive(execution_count) ? "Yes" : "No") << '\n';
    std::cout << "Is even?     " << (is_even(execution_count)     ? "Yes" : "No") << '\n';

    print_divider();

    Stats stats;
    record_run(stats);
    std::cout << "Total runs: " << stats.runs << '\n';

    std::cout << "3 * 4 = " << multiply(3, 4) << '\n';

    print_status("Program running normally");

    Config cfg;
    toggle_debug(cfg);
    std::cout << "Debug mode: " << (cfg.debug ? "ON" : "OFF") << '\n';

    std::cout << "square(5)    = " << square(5)    << '\n';
    std::cout << "is_zero(2)?    " << (is_zero(execution_count) ? "Yes" : "No") << '\n';
    std::cout << "absolute(-7) = " << absolute(-7) << '\n';
    std::cout << "max(10,20)   = " << max_value(10, 20) << '\n';
    std::cout << "factorial(5) = " << factorial(5)  << '\n';
    std::cout << "to_upper:      " << to_upper("hello world") << '\n';

    std::cout << "is_valid(50)? " << (is_valid_number(50) ? "Yes" : "No") << '\n';
    std::cout << "clamp(150,0,100) = " << clamp(150, 0, 100) << '\n';
    std::cout << "average(10,20)   = " << average(10, 20) << '\n';
    std::cout << "is_empty(\"\")? " << (is_empty("") ? "Yes" : "No") << '\n';

    print_divider();

    std::cout << "min(10,20)       = " << min_value(10, 20) << '\n';
    std::cout << "cube(3)          = " << cube(3) << '\n';
    std::cout << "reverse(OpenAI)  = " << reverse_string("OpenAI") << '\n';

    const std::vector<int> numbers = {1, 2, 3, 4, 5};
    std::cout << "Vector: ";        print_vector(numbers);
    std::cout << "sum    = " << sum_vector(numbers)    << '\n';
    std::cout << "max    = " << max_in_vector(numbers) << '\n';
    std::cout << "size   = " << vector_size(numbers)   << '\n';
    std::cout << "char_count(\"ChatGPT\") = " << char_count("ChatGPT") << '\n';

    print_divider();

    std::cout << "is_odd(2)?    " << (is_odd(execution_count) ? "Yes" : "No") << '\n';
    std::cout << "to_lower:     " << to_lower("HELLO WORLD") << '\n';

    static_assert(square(5)    == 25);
    static_assert(cube(3)      == 27);
    static_assert(factorial(5) == 120);
    static_assert(absolute(-7) == 7);
    static_assert(clamp(150, 0, 100) == 100);
    static_assert(add(5, 7)    == 12);

    std::cout << "\nAll static assertions passed.\n";

    print_divider();
    std::cout << "Program finished successfully.\n";
    say_goodbye();

    return 0;
}
