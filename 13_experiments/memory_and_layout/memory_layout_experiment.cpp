// Memory and Layout Experiment
// Understanding object layout, padding, and alignment

#include <iostream>
#include <cstring>
#include <cstddef>

// ✅ ADDED
#include <iomanip>   // for byte printing

struct Unaligned {
    char c;      // 1 byte
    int i;       // 4 bytes
    char d;      // 1 byte
};

struct Aligned {
    int i;       // 4 bytes
    char c;      // 1 byte
    char d;      // 1 byte
};

// Small packed example
#pragma pack(push, 1)
struct Packed {
    char c;
    int i;
    char d;
};
#pragma pack(pop)

// ✅ ADDED: Explicit alignment control
struct alignas(16) CustomAligned {
    char c;
    int i;
};

// ✅ ADDED: Better memory ordering
struct Optimized {
    int i;
    char c;
    char d;
};

// ✅ ADDED: Function to calculate padding
template<typename T>
void print_padding(const std::string& name) {
    size_t total = sizeof(T);
    size_t actual = sizeof(char) + sizeof(int) + sizeof(char);
    std::cout << "Padding in " << name << ": " 
              << (total - actual) << " bytes\n";
}

// ✅ ADDED: Memory dump
void dump_bytes(const void* ptr, size_t size) {
    const unsigned char* bytes = static_cast<const unsigned char*>(ptr);
    for (size_t i = 0; i < size; ++i) {
        std::cout << std::setw(2) << std::hex << (int)bytes[i] << " ";
    }
    std::cout << std::dec << "\n";
}

// -------- NEW SMALL ADDITIONS --------

// Compare struct sizes
void compare_sizes() {
    std::cout << "\n--- Size Comparison ---\n";
    std::cout << "Unaligned vs Aligned difference: "
              << (sizeof(Unaligned) - sizeof(Aligned)) << " bytes\n";
}

// Check alignment boundaries
template<typename T>
void check_alignment(const std::string& name) {
    std::cout << name << " alignment boundary: "
              << alignof(T) << " bytes\n";
}

// Initialize and dump meaningful data
void fill_and_dump() {
    Unaligned obj{};
    obj.c = 'A';
    obj.i = 123456;
    obj.d = 'Z';

    std::cout << "\nFilled object memory dump:\n";
    dump_bytes(&obj, sizeof(obj));
}

// -------------------------------------

int main() {

    std::cout << "Size of Unaligned: " << sizeof(Unaligned) << "\n";
    std::cout << "Size of Aligned: " << sizeof(Aligned) << "\n";
    std::cout << "Size of Packed: " << sizeof(Packed) << "\n";

    // ✅ ADDED
    std::cout << "Size of CustomAligned: " << sizeof(CustomAligned) << "\n";
    std::cout << "Size of Optimized: " << sizeof(Optimized) << "\n";

    std::cout << "\nAlignment requirements:\n";
    std::cout << "alignof(Unaligned): " << alignof(Unaligned) << "\n";
    std::cout << "alignof(Aligned): " << alignof(Aligned) << "\n";

    // ✅ ADDED
    std::cout << "alignof(CustomAligned): " << alignof(CustomAligned) << "\n";

    std::cout << "\nOffset in Unaligned:\n";
    std::cout << "  c: " << offsetof(Unaligned, c) << "\n";
    std::cout << "  i: " << offsetof(Unaligned, i) << "\n";
    std::cout << "  d: " << offsetof(Unaligned, d) << "\n";

    // ✅ ADDED: Padding info
    std::cout << "\nPadding Analysis:\n";
    print_padding<Unaligned>("Unaligned");
    print_padding<Aligned>("Aligned");
    print_padding<Packed>("Packed");

    // Small memory layout visualization
    Unaligned obj{};
    std::cout << "\nMemory addresses:\n";
    std::cout << "  &obj: " << &obj << "\n";
    std::cout << "  &obj.c: " << static_cast<void*>(&obj.c) << "\n";
    std::cout << "  &obj.i: " << static_cast<void*>(&obj.i) << "\n";
    std::cout << "  &obj.d: " << static_cast<void*>(&obj.d) << "\n";

    // ✅ ADDED: Raw memory view
    std::cout << "\nRaw memory dump (Unaligned):\n";
    dump_bytes(&obj, sizeof(obj));

    // -------- NEW FEATURE USAGE --------

    compare_sizes();

    std::cout << "\nAlignment checks:\n";
    check_alignment<Unaligned>("Unaligned");
    check_alignment<Aligned>("Aligned");
    check_alignment<Packed>("Packed");

    fill_and_dump();

    // ----------------------------------

    return 0;
}
