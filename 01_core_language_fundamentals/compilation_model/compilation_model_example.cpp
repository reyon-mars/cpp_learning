#include <iostream>
#include <string_view>
#include <bit>
#include <limits>

#define APP_VERSION "1.0"
#define APP_NAME    "CompilationDemo"
#define MAX_USERS   100

#ifdef NDEBUG
  #define DEBUG_BUILD false
#else
  #define DEBUG_BUILD true
#endif

// Macro retained to demonstrate its expansion pitfall
#define DOUBLE_MACRO(x) x + x

namespace compiler_demo {

[[nodiscard]] constexpr int square(int x)      noexcept { return x * x;     }
[[nodiscard]] constexpr int cube(int x)        noexcept { return x * x * x; }
[[nodiscard]] constexpr int double_safe(int x) noexcept { return x + x;     }

static_assert(square(5)      == 25);
static_assert(cube(2)        ==  8);
static_assert(double_safe(7) == 14);

inline int build_number    = 1;
inline int internal_counter = 0;

void print_version()  { std::cout << "Version: " << APP_VERSION << '\n'; }
void print_app_name() { std::cout << "App:     " << APP_NAME    << '\n'; }
void print_divider()  { std::cout << "-----------------------------\n";   }

void print_build_info() {
    std::cout << "Build date: " << __DATE__ << '\n';
    std::cout << "Build time: " << __TIME__ << '\n';
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
    if constexpr (DEBUG_BUILD) {
        std::cout << "[DEBUG] Debug build active\n";
    }
}

void external_function() {
    std::cout << "Simulated external function call\n";
}

namespace {
void internal_helper() {
    std::cout << "Internal (anonymous-namespace) helper\n";
}
} // anonymous namespace

void print_binary(unsigned int value) {
    std::cout << "Binary: ";
    for (int i = static_cast<int>(std::numeric_limits<unsigned int>::digits) - 1; i >= 0; --i)
        std::cout << ((value >> i) & 1u);
    std::cout << '\n';
}

void show_compilation_steps() {
    std::cout << "1. Preprocessing\n"
              << "2. Parsing\n"
              << "3. Compilation\n"
              << "4. Linking\n";
}

} // namespace compiler_demo

int main() {
    using namespace compiler_demo;

    std::cout << "Compiled successfully\n";
    print_divider();

    print_app_name();
    print_version();
    std::cout << "Build number: " << build_number << '\n';

    print_divider();
    print_build_info();
    print_compiler_info();

    print_divider();
    debug_message();

    ++internal_counter;
    std::cout << "Internal counter: " << internal_counter << '\n';

    print_divider();
    std::cout << "square(5)      = " << square(5)      << '\n';
    std::cout << "cube(3)        = " << cube(3)        << '\n';
    std::cout << "DOUBLE_MACRO(5*2) = " << DOUBLE_MACRO(5 * 2) << "  (macro pitfall)\n";
    std::cout << "double_safe(5*2)  = " << double_safe(5 * 2)  << '\n';

    print_divider();
    std::cout << "MAX_USERS = " << MAX_USERS << '\n';
    external_function();
    internal_helper();

    print_divider();
    constexpr int val      = square(6);
    constexpr int cube_val = cube(4);
    std::cout << "Compile-time square(6) = " << val      << '\n';
    std::cout << "Compile-time cube(4)   = " << cube_val << '\n';

    print_divider();
    std::cout << "Compilation stages:\n";
    show_compilation_steps();

    print_divider();
    std::cout << "Binary of 42:\n";
    print_binary(42u);

    print_divider();
    std::cout << "Compilation Model Summary:\n"
              << "  Macros     -> expanded during preprocessing\n"
              << "  Syntax     -> checked during parsing\n"
              << "  Code       -> translated during compilation\n"
              << "  Symbols    -> resolved during linking\n";

    print_divider();
    std::cout << "All static assertions passed.\n";

    return 0;
}
