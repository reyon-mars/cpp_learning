#include <iostream>
#include <memory> 
#include <utility>   
#include <cassert>   
#include <vector>    
#include <string>    

// ---------- SMALL ADDITIONS ----------

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

void debugMessage();

void printVersion() {
    std::cout << "App Version: " << APP_VERSION << "\n";
}

// Global variable (linking example)
int build_number = 1;

// Static variable (internal linkage)
static int internal_counter = 0;

: Internal global
static int internal_global = 999;

// Inline function
inline int square(int x) {
    return x * x;
}

// constexpr alternative
constexpr int square_constexpr(int x) {
    return x * x;
}

void debugMessage() {
#ifdef DEBUG_MODE
    std::cout << "[DEBUG] Debug mode is ON\n";
#endif
}

// Simulating extern usage
extern int build_number;

void externalFunction() {
    std::cout << "Simulated external function call\n";
}

// ------------------------------------


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

: stringify macro
#define STRINGIFY(x) #x

: token concatenation
#define CONCAT(a, b) a##b

// --------------------------------------


// ----------- STORAGE ADDITIONS -----------

// RAII example
void smart_pointer_example() {
    std::unique_ptr<int> ptr = std::make_unique<int>(200);

    assert(ptr);   safety check

    std::cout << "smart_pointer value: "
              << *ptr << '\n';
}

// Lifetime demo
void lifetime_demo() {
    std::cout << "Entering lifetime_demo\n";

    int temp = 123;

    std::cout << "temp exists: "
              << temp << '\n';

    std::cout << "Exiting lifetime_demo (temp destroyed)\n";
}

// Dangling pointer prevention
void safe_delete(int*& ptr) {
    delete ptr;
    ptr = nullptr;
}

: shared_ptr demo
void shared_pointer_example() {

    std::shared_ptr<int> ptr1 =
        std::make_shared<int>(300);

    std::shared_ptr<int> ptr2 = ptr1;

    std::cout << "shared_ptr value: "
              << *ptr1 << '\n';

    std::cout << "Reference count: "
              << ptr1.use_count() << '\n';
}

: weak_ptr demo
void weak_pointer_example() {

    std::shared_ptr<int> shared =
        std::make_shared<int>(999);

    std::weak_ptr<int> weak = shared;

    std::cout << "weak_ptr expired? "
              << (weak.expired() ? "Yes" : "No")
              << '\n';
}

: stack vs heap
void memory_region_demo() {

    int stackVar = 10;
    int* heapVar = new int(20);

    std::cout << "Stack variable: "
              << stackVar << '\n';

    std::cout << "Heap variable: "
              << *heapVar << '\n';

    delete heapVar;
}

: move semantics demo
void move_demo() {

    std::unique_ptr<int> ptr1 =
        std::make_unique<int>(555);

    std::unique_ptr<int> ptr2 =
        std::move(ptr1);

    if (!ptr1) {
        std::cout << "Ownership transferred using move\n";
    }

    std::cout << "Moved value: "
              << *ptr2 << '\n';
}

// ----------------------------------------


// ================= MAIN =================

int main() {

    std::cout << "Compiled successfully\n";

    printVersion();

    std::cout << "Build number: "
              << build_number << "\n";

    debugMessage();

    internal_counter++;

    std::cout << "Internal counter: "
              << internal_counter << "\n";

    std::cout << "Square of 5: "
              << square(5) << "\n";

    std::cout << "Square (macro): "
              << SQUARE_MACRO(5) << "\n";

    std::cout << "Square (constexpr): "
              << square_constexpr(5) << "\n";

    std::cout << "Max users (macro): "
              << MAX_USERS << "\n";

    externalFunction();

    // ================= MACRO FEATURES =================

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

    : stringify demo
    std::cout << "STRINGIFY(TestMacro): "
              << STRINGIFY(TestMacro) << "\n";

    : concatenation demo
    int CONCAT(my,Value) = 42;

    std::cout << "Concatenated variable: "
              << myValue << "\n";

    // ================= STORAGE FEATURES =================

    std::cout << "\nAdvanced Storage Concepts:\n";

    // internal linkage
    std::cout << "internal_global: "
              << internal_global << '\n';

    // smart pointer demo
    smart_pointer_example();

    // lifetime demo
    lifetime_demo();

    // shared_ptr demo
    shared_pointer_example();

    // weak_ptr demo
    weak_pointer_example();

    // stack vs heap demo
    memory_region_demo();

    // move semantics
    move_demo();

    // safe delete
    int* temp_ptr = new int(500);

    assert(temp_ptr != nullptr);  

    safe_delete(temp_ptr);

    if (temp_ptr == nullptr) {
        std::cout << "Pointer safely deleted and set to nullptr\n";
    }

    // ====================================================

    : vector dynamic storage
    std::vector<int> numbers = {1, 2, 3, 4, 5};

    std::cout << "\nVector contents:\n";

    for (int n : numbers) {
        std::cout << n << " ";
    }

    std::cout << "\n";

    : constexpr compile-time value
    constexpr int compileTimeSquare =
        square_constexpr(8);

    std::cout << "Compile-time square: "
              << compileTimeSquare << "\n";

    : address demonstration
    int localVar = 100;

    std::cout << "Address of localVar: "
              << &localVar << "\n";

    // ====================================================

    #undef DEBUG_MODE

    return 0;
}
