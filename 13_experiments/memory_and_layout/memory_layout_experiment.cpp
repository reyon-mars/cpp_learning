#include <iostream>
#include <cstring>
#include <cstddef>


#include <iomanip>      // for byte printing
#include <type_traits>  // NEW
#include <array>        // tiny addition
#include <numeric>      // tiny addition
#include <algorithm>    // tiny addition

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

: Explicit alignment control
struct alignas(16) CustomAligned {
    char c;
    int i;
};

: Better memory ordering
struct Optimized {
    int i;
    char c;
    char d;
};

: Function to calculate padding
template<typename T>
void print_padding(const std::string& name) {
    size_t total = sizeof(T);
    size_t actual = sizeof(char) + sizeof(int) + sizeof(char);

    std::cout << "Padding in " << name << ": "
              << (total - actual) << " bytes\n";
}

: Memory dump
void dump_bytes(const void* ptr, size_t size) {
    const unsigned char* bytes =
        static_cast<const unsigned char*>(ptr);

    for (size_t i = 0; i < size; ++i) {
        std::cout << std::setw(2)
                  << std::setfill('0')
                  << std::hex
                  << static_cast<int>(bytes[i])
                  << " ";
    }

    std::cout << std::dec << "\n";
}

// -------- NEW SMALL ADDITIONS --------

// Compare struct sizes
void compare_sizes() {
    std::cout << "\n--- Size Comparison ---\n";

    std::cout << "Unaligned vs Aligned difference: "
              << (sizeof(Unaligned) - sizeof(Aligned))
              << " bytes\n";
}

// Check alignment boundaries
template<typename T>
void check_alignment(const std::string& name) {
    std::cout << name << " alignment boundary: "
              << alignof(T)
              << " bytes\n";
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


// Check if type is trivially copyable
template<typename T>
void check_trivial(const std::string& name) {
    std::cout << name << " is trivially copyable? "
              << (std::is_trivially_copyable<T>::value ? "Yes" : "No")
              << "\n";
}

// Show address distance between members
void show_member_spacing() {
    Unaligned obj{};

    std::cout << "\nMember spacing (Unaligned):\n";

    std::cout << "c -> i: "
              << (reinterpret_cast<char*>(&obj.i)
                  - reinterpret_cast<char*>(&obj.c))
              << " bytes\n";

    std::cout << "i -> d: "
              << (reinterpret_cast<char*>(&obj.d)
                  - reinterpret_cast<char*>(&obj.i))
              << " bytes\n";
}

// Compare packed vs normal layout
void compare_layouts() {
    std::cout << "\n--- Layout Comparison ---\n";

    std::cout << "Packed saves "
              << (sizeof(Unaligned) - sizeof(Packed))
              << " bytes compared to Unaligned\n";
}

// ====================================

// ---- EXTRA SMALL ADDITIONS ----

// print divider
void print_divider() {
    std::cout << "-----------------------------\n";
}

// show total sizes
void show_total_sizes() {
    std::array<size_t, 5> sizes = {
        sizeof(Unaligned),
        sizeof(Aligned),
        sizeof(Packed),
        sizeof(CustomAligned),
        sizeof(Optimized)
    };

    size_t total =
        std::accumulate(sizes.begin(), sizes.end(), size_t{0});

    std::cout << "Combined struct sizes: "
              << total << " bytes\n";
}

// find largest structure
void largest_structure() {
    std::array<size_t, 5> sizes = {
        sizeof(Unaligned),
        sizeof(Aligned),
        sizeof(Packed),
        sizeof(CustomAligned),
        sizeof(Optimized)
    };

    auto max_size =
        *std::max_element(sizes.begin(), sizes.end());

    std::cout << "Largest structure size: "
              << max_size << " bytes\n";
}

// =====================================

int main() {

    std::cout << "Size of Unaligned: "
              << sizeof(Unaligned) << "\n";

    std::cout << "Size of Aligned: "
              << sizeof(Aligned) << "\n";

    std::cout << "Size of Packed: "
              << sizeof(Packed) << "\n";

    
    std::cout << "Size of CustomAligned: "
              << sizeof(CustomAligned) << "\n";

    std::cout << "Size of Optimized: "
              << sizeof(Optimized) << "\n";

    std::cout << "\nAlignment requirements:\n";

    std::cout << "alignof(Unaligned): "
              << alignof(Unaligned) << "\n";

    std::cout << "alignof(Aligned): "
              << alignof(Aligned) << "\n";

    
    std::cout << "alignof(CustomAligned): "
              << alignof(CustomAligned) << "\n";

    std::cout << "\nOffset in Unaligned:\n";

    std::cout << "  c: "
              << offsetof(Unaligned, c) << "\n";

    std::cout << "  i: "
              << offsetof(Unaligned, i) << "\n";

    std::cout << "  d: "
              << offsetof(Unaligned, d) << "\n";

    : Padding info
    std::cout << "\nPadding Analysis:\n";

    print_padding<Unaligned>("Unaligned");
    print_padding<Aligned>("Aligned");
    print_padding<Packed>("Packed");

    // Small memory layout visualization
    Unaligned obj{};

    std::cout << "\nMemory addresses:\n";

    std::cout << "  &obj: "
              << &obj << "\n";

    std::cout << "  &obj.c: "
              << static_cast<void*>(&obj.c) << "\n";

    std::cout << "  &obj.i: "
              << static_cast<void*>(&obj.i) << "\n";

    std::cout << "  &obj.d: "
              << static_cast<void*>(&obj.d) << "\n";

    : Raw memory view
    std::cout << "\nRaw memory dump (Unaligned):\n";
    dump_bytes(&obj, sizeof(obj));

    // -------- NEW FEATURE USAGE --------

    compare_sizes();

    std::cout << "\nAlignment checks:\n";

    check_alignment<Unaligned>("Unaligned");
    check_alignment<Aligned>("Aligned");
    check_alignment<Packed>("Packed");

    fill_and_dump();

    // ===== NEW USAGE =====

    check_trivial<Unaligned>("Unaligned");
    check_trivial<Packed>("Packed");

    show_member_spacing();
    compare_layouts();

    // ---- EXTRA SMALL USAGE ----

    print_divider();

    show_total_sizes();
    largest_structure();

    std::cout << "Packed struct uses least memory? "
              << ((sizeof(Packed) <= sizeof(Unaligned)) ? "Yes" : "No")
              << "\n";

    std::cout << "Optimized <= Unaligned? "
              << ((sizeof(Optimized) <= sizeof(Unaligned)) ? "Yes" : "No")
              << "\n";

    // ===================================

    // ----------------------------------

    return 0;
}
