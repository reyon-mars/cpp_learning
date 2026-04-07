// ----------- MORE ADVANCED ADDITIONS -----------

#include <memory> // for smart pointers

// Internal linkage example
static int internal_global = 999;

// RAII example (automatic memory management)
void smart_pointer_example() {
    std::unique_ptr<int> ptr = std::make_unique<int>(200);
    std::cout << "smart_pointer value: " << *ptr << '\n';
} // automatically freed here

// Lifetime demonstration
void lifetime_demo() {
    std::cout << "Entering lifetime_demo\n";
    int temp = 123;
    std::cout << "temp exists: " << temp << '\n';
    std::cout << "Exiting lifetime_demo (temp destroyed)\n";
}

// Dangling pointer prevention example
void safe_delete(int*& ptr) {
    delete ptr;
    ptr = nullptr;
}

// ----------------------------------------------


// ================= ADD IN MAIN =================

// (Add near the end before return)

std::cout << "\nAdvanced Storage Concepts:\n";

// ✅ ADDED: internal linkage
std::cout << "internal_global: " << internal_global << '\n';

// ✅ ADDED: smart pointer demo
smart_pointer_example();

// ✅ ADDED: lifetime demo
lifetime_demo();

// ✅ ADDED: safe delete
int* temp_ptr = new int(500);
safe_delete(temp_ptr);

if (temp_ptr == nullptr) {
    std::cout << "Pointer safely deleted and set to nullptr\n";
}
