// Compilation Model Exercise
// Understanding preprocessing, parsing, compilation, and linking

#include <iostream>
#include <string>   
#include <vector>   
#include <array>    // ✅ NEW
#include <functional> // ✅ NEW

// Preprocessor macro
#define APP_VERSION "1.0"

// Conditional compilation
#define DEBUG_MODE

// Header guard simulation
#ifndef CONFIG_H
#define CONFIG_H
#define MAX_USERS 100
#endif

// Macro function
#define SQUARE_MACRO(x) ((x) * (x))

// Function prototype (parsing stage example)
void debugMessage();

// Simple helper function (compiled separately conceptually)
void printVersion() {
    std::cout << "App Version: " << APP_VERSION << "\n";
}

// Global variable (linking example)
int build_number = 1;

// Static variable (internal linkage)
static int internal_counter = 0;

// Inline function (compiler optimization)
inline int square(int x) {
    return x * x;
}

// constexpr alternative (better than macro)
constexpr int square_constexpr(int x) {
    return x * x;
}

// Function definition (after prototype)
void debugMessage() {
#ifdef DEBUG_MODE
    std::cout << "[DEBUG] Debug mode is ON\n";
#endif
}

// Simulating extern usage
extern int build_number;

// extern function simulation
void externalFunction() {
    std::cout << "Simulated external function call\n";
}


// Safer alternative to macro (inline function)
inline int max_safe(int a, int b) {
    return (a > b) ? a : b;
}

// Macro with file and line info
#define DEBUG_LOG(msg) \
    std::cout << "[DEBUG] " << msg \
              << " (" << __FILE__ << ":" << __LINE__ << ")\n"

// Conditional compilation using defined
#if defined(DEBUG_MODE)
    #define MODE_STATUS "Debug Mode Active"
#else
    #define MODE_STATUS "Release Mode"
#endif

// Macro pitfall example
#define BAD_SQUARE(x) x * x

: Stringify macro
#define STRINGIFY(x) #x

: Token pasting macro
#define CONCAT(a, b) a##b

: Multi-line macro
#define PRINT_SEPARATOR() \
    std::cout << "-----------------------------\n"

: Compiler info macro
#define COMPILER_INFO "Compiled with standard preprocessing"

: Simple constexpr math
constexpr int cube_constexpr(int x) {
    return x * x * x;
}

: Internal linkage helper
static void internalHelper() {
    std::cout << "Internal helper executed\n";
}

: Simulated translation unit info
void printCompilationStages() {

    std::vector<std::string> stages = {
        "1. Preprocessing",
        "2. Parsing",
        "3. Compilation",
        "4. Assembly",
        "5. Linking"
    };

    std::cout << "\nCompilation Stages:\n";

    for (const auto& stage : stages) {
        std::cout << stage << "\n";
    }
}

: constexpr compile-time test
constexpr int compileTimeValue = square_constexpr(10);

// ====================================================
// ✅ NEW ADDITIONS (Original Code Unchanged)
// ====================================================

// Namespace example
namespace CompilerDemo {
    void showNamespaceInfo() {
        std::cout << "Namespace function executed\n";
    }
}

// Enum class
enum class BuildType {
    Debug,
    Release
};

// Function pointer target
int multiply(int a, int b) {
    return a * b;
}

// Compile-time array
constexpr std::array<int, 5> compileArray = {1,2,3,4,5};

// Template function
template<typename T>
void printValue(T value) {
    std::cout << value << "\n";
}

// Lambda demonstration
void lambdaDemo() {
    auto greet = []() {
        std::cout << "Lambda function executed\n";
    };

    greet();
}

// ====================================================


int main() {

    std::cout << "Compiled successfully\n";

    // Added usage
    printVersion();

    std::cout << "Build number: "
              << build_number << "\n";

    debugMessage();

    internal_counter++;

    std::cout << "Internal counter: "
              << internal_counter << "\n";

    std::cout << "Square of 5: "
              << square(5) << "\n";

    // Macro vs constexpr
    std::cout << "Square (macro): "
              << SQUARE_MACRO(5) << "\n";

    std::cout << "Square (constexpr): "
              << square_constexpr(5) << "\n";

    // Config usage
    std::cout << "Max users (macro): "
              << MAX_USERS << "\n";

    // External function simulation
    externalFunction();

    std::cout << "\nAdvanced Macro Features:\n";

    std::cout << "max_safe(5,10): "
              << max_safe(5,10) << "\n";

    DEBUG_LOG("Testing debug log");

    std::cout << "Mode: "
              << MODE_STATUS << "\n";

    std::cout << "BAD_SQUARE(2+3): "
              << BAD_SQUARE(2+3) << "\n";

    std::cout << "Correct SQUARE(2+3): "
              << SQUARE_MACRO(2+3) << "\n";

    // --------------------------------------------------

    std::cout << "\nStringify Macro:\n";
    std::cout << STRINGIFY(Hello Compilation Model)
              << "\n";

    // --------------------------------------------------

    int CONCAT(my,Var) = 25;

    std::cout << "Concatenated variable value: "
              << myVar << "\n";

    // --------------------------------------------------

    PRINT_SEPARATOR();

    // --------------------------------------------------

    std::cout << "Cube constexpr of 3: "
              << cube_constexpr(3) << "\n";

    // --------------------------------------------------

    std::cout << "Compile-time value: "
              << compileTimeValue << "\n";

    // --------------------------------------------------

    internalHelper();

    // --------------------------------------------------

    printCompilationStages();

    // --------------------------------------------------

    std::cout << "\nCompiler Info:\n";
    std::cout << COMPILER_INFO << "\n";

    // --------------------------------------------------

    std::cout << "\nBuilt-in Macros:\n";

    std::cout << "__LINE__: "
              << __LINE__ << "\n";

    std::cout << "__FILE__: "
              << __FILE__ << "\n";

    std::cout << "__cplusplus: "
              << __cplusplus << "\n";

    // ====================================================
    // ✅ NEWLY ADDED EXECUTION SECTION
    // ====================================================

    PRINT_SEPARATOR();

    std::cout << "\nNamespace Example:\n";
    CompilerDemo::showNamespaceInfo();

    PRINT_SEPARATOR();

    std::cout << "\nEnum Class Example:\n";
    BuildType build = BuildType::Debug;

    if (build == BuildType::Debug) {
        std::cout << "Current build type: Debug\n";
    }

    PRINT_SEPARATOR();

    std::cout << "\nFunction Pointer Example:\n";
    int (*funcPtr)(int, int) = multiply;

    std::cout << "6 * 7 = "
              << funcPtr(6, 7)
              << "\n";

    PRINT_SEPARATOR();

    std::cout << "\nCompile-Time Array:\n";

    for (auto value : compileArray) {
        std::cout << value << " ";
    }

    std::cout << "\n";

    PRINT_SEPARATOR();

    std::cout << "\nTemplate Function Example:\n";
    printValue(100);
    printValue(std::string("Template Works"));

    PRINT_SEPARATOR();

    std::cout << "\nLambda Demonstration:\n";
    lambdaDemo();

    PRINT_SEPARATOR();

    std::cout << "\nAdditional Predefined Macros:\n";

    std::cout << "__DATE__: "
              << __DATE__ << "\n";

    std::cout << "__TIME__: "
              << __TIME__ << "\n";

    // ====================================================

    // undef example
    #undef DEBUG_MODE
    // debugMessage(); // would not print now if called again

    return 0;
}
