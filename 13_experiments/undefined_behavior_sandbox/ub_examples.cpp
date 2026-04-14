// Undefined Behavior Sandbox
// Exploring and understanding undefined behavior

#include <iostream>
#include <climits>

// ✅ ADDED
#include <vector>
#include <optional>

// -------- NEW ADDITIONS --------
#include <array>

// Safe division helper
void safe_division(int a, int b) {
    if (b == 0) {
        std::cout << "Division by zero avoided (would be UB)\n";
        return;
    }
    std::cout << "Division result: " << a / b << "\n";
}

// --------------------------------

int main() {

    // Example 1: Buffer overflow
    // char buffer[5];
    // strcpy(buffer, "Hello World");  // UB: exceeds buffer size

    // Example 2: Null pointer dereference
    // int* ptr = nullptr;
    // *ptr = 10;  // UB

    // Example 3: Use after free
    // int* p = new int(10);
    // delete p;
    // *p = 20;  // UB

    // Example 4: Signed integer overflow
    // int max_int = INT_MAX;
    // int overflow = max_int + 1;  // UB in C++

    std::cout << "Undefined behavior examples (commented out for safety)\n\n";

    // ---- Safe experiments ----

    // Demonstrating integer limits
    std::cout << "INT_MAX: " << INT_MAX << "\n";
    std::cout << "INT_MIN: " << INT_MIN << "\n";

    // Safe overflow check
    int a = INT_MAX;

    if (a > INT_MAX - 1) {
        std::cout << "Adding 1 would cause signed integer overflow (UB)\n";
    }

    // Dangling pointer example (without dereferencing)
    int* ptr = new int(42);
    delete ptr;

    std::cout << "Pointer after delete (dangling): " << ptr << "\n";
    std::cout << "Accessing it would be undefined behavior\n";

    // ✅ ADDED: Best practice (null after delete)
    ptr = nullptr;
    std::cout << "Pointer reset to nullptr (safe practice)\n";

    // ----------------------------------------------------
    // ✅ ADDED: Uninitialized variable awareness (safe note)
    std::cout << "\nUninitialized variables can cause UB if used.\n";
    int x; 
    std::cout << "Uninitialized variable declared (not used to avoid UB)\n";

    // ----------------------------------------------------
    // ✅ ADDED: Safe array access
    std::cout << "\nSafe array access using vector::at():\n";
    std::vector<int> v = {1, 2, 3};

    try {
        std::cout << "Access v.at(5): ";
        std::cout << v.at(5) << "\n"; // throws exception instead of UB
    } catch (const std::out_of_range& e) {
        std::cout << "Caught exception: out_of_range (safe handling)\n";
    }

    // ----------------------------------------------------
    // ✅ ADDED: Using std::optional to avoid invalid states
    std::cout << "\nUsing std::optional:\n";
    std::optional<int> safe_value;

    if (!safe_value.has_value()) {
        std::cout << "Value not initialized, avoiding UB\n";
    }

    safe_value = 10;
    std::cout << "Safe value: " << safe_value.value() << "\n";

    // ----------------------------------------------------
    // ✅ NEW: std::array vs raw array
    std::cout << "\nstd::array vs raw array:\n";
    std::array<int, 3> arr = {1, 2, 3};

    try {
        std::cout << "arr.at(5): ";
        std::cout << arr.at(5) << "\n";
    } catch (...) {
        std::cout << "Out-of-bounds prevented using std::array\n";
    }

    // ----------------------------------------------------
    // ✅ NEW: Division by zero safety
    std::cout << "\nDivision safety:\n";
    safe_division(10, 2);
    safe_division(10, 0);

    // ----------------------------------------------------
    // ✅ NEW: Iterator invalidation awareness
    std::cout << "\nIterator invalidation awareness:\n";
    std::vector<int> v2 = {1, 2, 3};
    auto it = v2.begin();

    v2.push_back(4); // may invalidate iterator
    std::cout << "After push_back, iterator may be invalid\n";

    // ----------------------------------------------------
    // ✅ NEW: Signed vs unsigned mismatch
    std::cout << "\nSigned vs unsigned example:\n";
    int neg = -1;
    unsigned int u = 1;

    if (neg < u) {
        std::cout << "Unexpected comparison result due to type conversion\n";
    }

    // ----------------------------------------------------
    // ✅ NEW: Alignment awareness
    std::cout << "\nAlignment info:\n";
    std::cout << "Alignment of int: " << alignof(int) << "\n";
    std::cout << "Alignment of double: " << alignof(double) << "\n";

    return 0;
}
