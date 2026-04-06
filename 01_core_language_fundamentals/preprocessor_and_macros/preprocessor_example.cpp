// ----------- MORE ADVANCED ADDITIONS -----------

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
#define BAD_SQUARE(x) x * x  // missing parentheses!

// ----------------------------------------------


// ================= ADD IN MAIN =================

// (Add near the end before return)

std::cout << "\nAdvanced Macro Features:\n";

// ✅ ADDED: safe vs macro
std::cout << "max_safe(5,10): " << max_safe(5,10) << "\n";

// ✅ ADDED: debug with file/line
DEBUG_LOG("Testing debug log");

// ✅ ADDED: mode status
std::cout << "Mode: " << MODE_STATUS << "\n";

// ✅ ADDED: macro pitfall demo
std::cout << "BAD_SQUARE(2+3): " << BAD_SQUARE(2+3) << "\n";
std::cout << "Correct SQUARE(2+3): " << SQUARE(2+3) << "\n";
