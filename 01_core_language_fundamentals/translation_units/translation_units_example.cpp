// ----------- MORE ADVANCED ADDITIONS -----------

// Simulating external variable from another translation unit
extern int external_counter;

// Define it here (normally in another .cpp)
int external_counter = 5;

// Anonymous namespace (modern internal linkage)
namespace {
    void hidden_function() {
        std::cout << "Hidden function (anonymous namespace)\n";
    }
}

// Inline function (safe across multiple translation units)
inline void inline_helper() {
    std::cout << "Inline helper function\n";
}

// Function modifying external variable
void modify_external() {
    external_counter += 10;
    std::cout << "Modified external_counter: "
              << external_counter << "\n";
}

// ----------------------------------------------


// ================= ADD IN MAIN =================

// (Add near the end before return)

std::cout << "\nAdvanced Translation Unit Concepts:\n";

// ✅ Anonymous namespace
hidden_function();

// ✅ Inline function
inline_helper();

// ✅ External variable usage
std::cout << "external_counter: "
          << external_counter << "\n";

modify_external();
