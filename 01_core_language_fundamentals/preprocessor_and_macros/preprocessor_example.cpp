#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <array>
#include <functional>
#include <format>

#define APP_VERSION "1.0"
#define DEBUG_MODE

#ifndef CONFIG_H
#define CONFIG_H
inline constexpr int max_users = 100;
#endif

#define SQUARE_MACRO(x) ((x) * (x))
#define BAD_SQUARE(x) x * x
#define STRINGIFY(x) #x
#define CONCAT(a, b) a##b
#define DEBUG_LOG(msg) \
    std::cout << std::format("[DEBUG] {} ({}:{})\n", msg, __FILE__, __LINE__)
#define PRINT_SEPARATOR() std::cout << "-----------------------------\n"

#if defined(DEBUG_MODE)
inline constexpr std::string_view mode_status = "Debug Mode Active";
#else
inline constexpr std::string_view mode_status = "Release Mode";
#endif

inline constexpr std::string_view compiler_info = "Compiled with standard preprocessing";

void debug_message();

void print_version() {
    std::cout << std::format("App Version: {}\n", APP_VERSION);
}

int build_number = 1;

namespace {
int internal_counter = 0;

void internal_helper() {
    std::cout << "Internal helper executed\n";
}
}

[[nodiscard]] constexpr int square(int x) noexcept { return x * x; }
[[nodiscard]] constexpr int cube(int x)   noexcept { return x * x * x; }
[[nodiscard]] constexpr int max_safe(int a, int b) noexcept { return a > b ? a : b; }

void debug_message() {
#ifdef DEBUG_MODE
    std::cout << "[DEBUG] Debug mode is ON\n";
#endif
}

extern int build_number;

void external_function() {
    std::cout << "Simulated external function call\n";
}

void print_compilation_stages() {
    constexpr std::array stages{
        "1. Preprocessing",
        "2. Parsing",
        "3. Compilation",
        "4. Assembly",
        "5. Linking",
    };

    std::cout << "\nCompilation Stages:\n";
    for (const auto& stage : stages) std::cout << stage << '\n';
}

inline constexpr int compile_time_value = square(10);

namespace CompilerDemo {
void show_namespace_info() {
    std::cout << "Namespace function executed\n";
}
}

enum class BuildType { Debug, Release };

[[nodiscard]] constexpr int multiply(int a, int b) noexcept { return a * b; }

inline constexpr std::array compile_array{1, 2, 3, 4, 5};

template <typename T>
void print_value(const T& value) {
    std::cout << value << '\n';
}

void lambda_demo() {
    const auto greet = [] { std::cout << "Lambda function executed\n"; };
    greet();
}

int main() {
    std::cout << "Compiled successfully\n";

    print_version();
    std::cout << std::format("Build number: {}\n", build_number);

    debug_message();

    ++internal_counter;
    std::cout << std::format("Internal counter: {}\n", internal_counter);

    std::cout << std::format("Square of 5 (constexpr): {}\n", square(5));
    std::cout << std::format("Square of 5 (macro):     {}\n", SQUARE_MACRO(5));
    std::cout << std::format("Max users: {}\n", max_users);

    external_function();

    std::cout << "\nMacros vs. modern alternatives:\n";
    std::cout << std::format("max_safe(5,10):           {}\n", max_safe(5, 10));
    DEBUG_LOG("Testing debug log");
    std::cout << std::format("Mode: {}\n", mode_status);

    std::cout << std::format("BAD_SQUARE(2+3) [buggy]:  {}\n", BAD_SQUARE(2 + 3));
    std::cout << std::format("SQUARE_MACRO(2+3) [safe]: {}\n", SQUARE_MACRO(2 + 3));
    std::cout << std::format("square(2+3) [constexpr]:  {}\n", square(2 + 3));

    std::cout << "\nStringify macro:\n";
    std::cout << STRINGIFY(Hello Compilation Model) << '\n';

    int CONCAT(my, Var) = 25;
    std::cout << std::format("Concatenated variable value: {}\n", myVar);

    PRINT_SEPARATOR();

    std::cout << std::format("Cube of 3:           {}\n", cube(3));
    std::cout << std::format("Compile-time value:  {}\n", compile_time_value);

    internal_helper();

    PRINT_SEPARATOR();

    print_compilation_stages();

    std::cout << "\nCompiler info:\n" << compiler_info << '\n';

    std::cout << "\nBuilt-in macros:\n";
    std::cout << std::format("__LINE__:     {}\n", __LINE__);
    std::cout << std::format("__FILE__:     {}\n", __FILE__);
    std::cout << std::format("__cplusplus:  {}\n", __cplusplus);
    std::cout << std::format("__DATE__:     {}\n", __DATE__);
    std::cout << std::format("__TIME__:     {}\n", __TIME__);

    PRINT_SEPARATOR();

    std::cout << "\nNamespace example:\n";
    CompilerDemo::show_namespace_info();

    PRINT_SEPARATOR();

    std::cout << "\nEnum class example:\n";
    if (constexpr auto build = BuildType::Debug; build == BuildType::Debug)
        std::cout << "Current build type: Debug\n";

    PRINT_SEPARATOR();

    std::cout << "\nFunction pointer example:\n";
    const std::function<int(int, int)> func_ptr = multiply;
    std::cout << std::format("6 * 7 = {}\n", func_ptr(6, 7));

    PRINT_SEPARATOR();

    std::cout << "\nCompile-time array:\n";
    for (const auto value : compile_array) std::cout << value << ' ';
    std::cout << '\n';

    PRINT_SEPARATOR();

    std::cout << "\nTemplate function example:\n";
    print_value(100);
    print_value(std::string_view{"Template works"});

    PRINT_SEPARATOR();

    std::cout << "\nLambda demonstration:\n";
    lambda_demo();

    PRINT_SEPARATOR();

#undef DEBUG_MODE

    return 0;
}
