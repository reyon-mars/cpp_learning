#include <iostream>
#include <memory>
#include <utility>   // ✅ ADDED
#include <cassert>   // ✅ ADDED

// ---------- SMALL ADDITIONS ----------

#define APP_VERSION "1.0"
#define DEBUG_MODE

#ifndef CONFIG_H
#define CONFIG_H
#define MAX_USERS 100
#endif

#define SQUARE_MACRO(x) ((x) * (x))

void debugMessage();

void printVersion() {
    std::cout << "App Version: " << APP_VERSION << "\n";
}

int build_number = 1;

static int internal_counter = 0;
static int internal_global = 999;

inline int square(int x) {
    return x * x;
}

constexpr int square_constexpr(int x) {
    return x * x;
}

void debugMessage() {
#ifdef DEBUG_MODE
    std::cout << "[DEBUG] Debug mode is ON\n";
#endif
}

extern int build_number;

void externalFunction() {
    std::cout << "Simulated external function call\n";
}

// ----------- MACRO ADDITIONS -----------

inline int max_safe(int a, int b) {
    return (a > b) ? a : b;
}

#define DEBUG_LOG(msg) \
    std::cout << "[DEBUG] " << msg \
              << " (" << __FILE__ << ":" << __LINE__ << ")\n"

#if defined(DEBUG_MODE)
    #define MODE_STATUS "Debug Mode Active"
#else
    #define MODE_STATUS "Release Mode"
#endif

#define BAD_SQUARE(x) x * x

// ----------- STORAGE ADDITIONS -----------

void smart_pointer_example() {
    std::unique_ptr<int> ptr = std::make_unique<int>(200);
    assert(ptr);  // ✅ ADDED
    std::cout << "smart_pointer value: " << *ptr << '\n';
}

void lifetime_demo() {
    std::cout << "Entering lifetime_demo\n";
    int temp = 123;
    std::cout << "temp exists: " << temp << '\n';
    std::cout << "Exiting lifetime_demo (temp destroyed)\n";
}

void safe_delete(int*& ptr) {
    delete ptr;
    ptr = nullptr;
}

// ----------- NEW: TRANSLATION UNIT ADDITIONS -----------

// Simulating external variable
extern int external_counter;

// Definition (normally separate .cpp)
int external_counter = 5;

// Anonymous namespace (internal linkage)
namespace {
    void hidden_function() {
        std::cout << "Hidden function (anonymous namespace)\n";
    }
}

// Inline helper
inline void inline_helper() {
    std::cout << "Inline helper function\n";
}

// Modify external variable
void modify_external() {
    external_counter += 10;
    std::cout << "Modified external_counter: "
              << external_counter << "\n";
}

// ------------------------------------------------------


int main() {
    std::cout << "Compiled successfully\n";

    printVersion();
    std::cout << "Build number: " << build_number << "\n";

    debugMessage();

    internal_counter++;
    std::cout << "Internal counter: " << internal_counter << "\n";

    std::cout << "Square of 5: " << square(5) << "\n";

    std::cout << "Square (macro): " << SQUARE_MACRO(5) << "\n";
    std::cout << "Square (constexpr): " << square_constexpr(5) << "\n";

    std::cout << "Max users (macro): " << MAX_USERS << "\n";

    externalFunction();

    // ================= MACRO FEATURES =================

    std::cout << "\nAdvanced Macro Features:\n";

    std::cout << "max_safe(5,10): " << max_safe(5,10) << "\n";

    DEBUG_LOG("Testing debug log");

    std::cout << "Mode: " << MODE_STATUS << "\n";

    std::cout << "BAD_SQUARE(2+3): " << BAD_SQUARE(2+3) << "\n";
    std::cout << "Correct SQUARE(2+3): " << SQUARE_MACRO(2+3) << "\n";

    // ================= STORAGE FEATURES =================

    std::cout << "\nAdvanced Storage Concepts:\n";

    std::cout << "internal_global: " << internal_global << '\n';

    smart_pointer_example();
    lifetime_demo();

    int* temp_ptr = new int(500);
    assert(temp_ptr != nullptr);  // ✅ ADDED

    safe_delete(temp_ptr);

    if (temp_ptr == nullptr) {
        std::cout << "Pointer safely deleted and set to nullptr\n";
    }

    // ================= NEW: TRANSLATION UNIT FEATURES =================

    std::cout << "\nAdvanced Translation Unit Concepts:\n";

    // Anonymous namespace
    hidden_function();

    // Inline function
    inline_helper();

    // External variable usage
    std::cout << "external_counter: "
              << external_counter << "\n";

    modify_external();

    // ================================================================

    #undef DEBUG_MODE

    return 0;
}
