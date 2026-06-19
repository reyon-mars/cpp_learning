// ----------- MORE ADVANCED ADDITIONS -----------

#include <iostream>      
#include <type_traits>   
#include <cstddef>       // ✅ NEW
#include <iomanip>       // ✅ NEW
#include <cassert>       // ✅ NEW
#include <string>        // ✅ NEW

: Packed struct (from earlier context)
#pragma pack(push, 1)
struct Packed {
    char c;
    int i;
    char d;
};
#pragma pack(pop)

: Optimized struct
struct Optimized {
    int i;
    char c;
    char d;
};

// Struct with manual alignment
struct alignas(16) Aligned16 {
    int x;
    double y;
};

// Calculate padding manually
template <typename T>
void show_padding() {
    std::cout << "Total size: " << sizeof(T)
              << ", alignment: " << alignof(T) << "\n";
}

// --------------------------------------------------
// ✅ NEW SMALL ADDITIONS
// --------------------------------------------------

// Unaligned struct for comparison
struct Unaligned {
    char a;
    int b;
    char c;
};

// ✅ EXTRA ADDITION
struct Compact {
    double value;
    int id;
    char flag;
};

// Memory layout printer
template<typename T>
void print_layout(const std::string& name) {

    std::cout << "\nLayout of "
              << name << ":\n";

    std::cout << "Size: "
              << sizeof(T) << "\n";

    std::cout << "Alignment: "
              << alignof(T) << "\n";
}

// Offset printer
void print_offsets() {

    std::cout << "\nOffsets in Unaligned:\n";

    std::cout << "a: "
              << offsetof(Unaligned, a) << "\n";

    std::cout << "b: "
              << offsetof(Unaligned, b) << "\n";

    std::cout << "c: "
              << offsetof(Unaligned, c) << "\n";
}

// Dump raw memory bytes
void dump_bytes(const void* ptr, size_t size) {

    const unsigned char* bytes =
        static_cast<const unsigned char*>(ptr);

    for (size_t i = 0; i < size; ++i) {

        std::cout
            << std::hex
            << std::setw(2)
            << std::setfill('0')
            << static_cast<int>(bytes[i])
            << " ";
    }

    std::cout << std::dec << "\n";
}

// Compare structure sizes
void compare_sizes() {

    std::cout << "\nStructure Comparison:\n";

    std::cout << "Packed size: "
              << sizeof(Packed) << "\n";

    std::cout << "Unaligned size: "
              << sizeof(Unaligned) << "\n";

    std::cout << "Optimized size: "
              << sizeof(Optimized) << "\n";
}

// ✅ EXTRA ADDITION
template<typename T>
void print_alignment_gap(const std::string& name) {

    std::cout << name
              << " alignment remainder: "
              << (sizeof(T) % alignof(T))
              << "\n";
}

// ✅ EXTRA ADDITION
void print_addresses(const Packed& p) {

    std::cout << "\nPacked member addresses:\n";

    std::cout << "&c = "
              << static_cast<const void*>(&p.c)
              << "\n";

    std::cout << "&i = "
              << static_cast<const void*>(&p.i)
              << "\n";

    std::cout << "&d = "
              << static_cast<const void*>(&p.d)
              << "\n";
}

// ----------------------------------------------

int main() {

    std::cout << "\nAdvanced Layout Concepts:\n";

    // ✅ Padding insight
    std::cout << "Padding in Packed: "
              << sizeof(Packed) - (sizeof(char) + sizeof(int) + sizeof(char))
              << "\n";

    // ✅ Optimized struct check
    std::cout << "Optimized alignment: "
              << alignof(Optimized) << "\n";

    // ✅ Trivial copy check
    std::cout << "Packed is trivially copyable? "
              << std::is_trivially_copyable<Packed>::value << "\n";

    // ✅ Custom alignment
    Aligned16 a16{1, 2.5};

    std::cout << "Aligned16 size: "
              << sizeof(Aligned16)
              << ", alignment: "
              << alignof(Aligned16)
              << "\n";

    // ✅ Generic padding display
    show_padding<Optimized>();

    // --------------------------------------------------
    // ✅ NEW FEATURE USAGE
    // --------------------------------------------------

    print_layout<Packed>("Packed");
    print_layout<Unaligned>("Unaligned");
    print_layout<Aligned16>("Aligned16");
    print_layout<Compact>("Compact");

    print_offsets();

    compare_sizes();

    // Memory dump demo
    Packed p{'A', 12345, 'Z'};

    std::cout << "\nPacked memory dump:\n";
    dump_bytes(&p, sizeof(p));

    // Address alignment check
    std::cout << "\nAddress checks:\n";

    std::cout << "&a16 = "
              << &a16 << "\n";

    std::cout << "&p = "
              << &p << "\n";

    // Alignment validation
    std::cout << "\nAligned16 properly aligned? "
              << (reinterpret_cast<std::uintptr_t>(&a16) % 16 == 0)
              << "\n";

    // Static assertions
    static_assert(alignof(Aligned16) == 16,
                  "Aligned16 should have 16-byte alignment");

    static_assert(sizeof(Packed) < sizeof(Unaligned),
                  "Packed should use less memory");

    // Standard layout checks
    std::cout << "\nType Traits:\n";

    std::cout << "Optimized is standard layout? "
              << std::is_standard_layout<Optimized>::value
              << "\n";

    std::cout << "Packed is POD-like? "
              << std::is_trivial<Packed>::value
              << "\n";

    // --------------------------------------------------
    // ✅ EXTRA FEATURE USAGE (ADDED WITHOUT CHANGING ORIGINAL LOGIC)
    // --------------------------------------------------

    print_alignment_gap<Packed>("Packed");
    print_alignment_gap<Optimized>("Optimized");
    print_alignment_gap<Aligned16>("Aligned16");

    print_addresses(p);

    Compact comp{3.14, 101, 'Y'};

    std::cout << "\nCompact struct:\n";
    std::cout << "value = " << comp.value
              << ", id = " << comp.id
              << ", flag = " << comp.flag
              << "\n";

    assert(sizeof(Compact) >= sizeof(double));

    // --------------------------------------------------

    return 0;
}
