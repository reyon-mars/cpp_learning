// Memory Layout and ABI Exercise
// Exploring object layout and padding

#include <iostream>
#include <cstddef>   // for offsetof
#include <type_traits>

// ---------------- ORIGINAL STRUCTS ----------------

struct Packed {
    char c1;
    int i;
    char c2;
};

#pragma pack(push, 1)
struct TightlyPacked {
    char c1;
    int i;
    char c2;
};
#pragma pack(pop)

// ----------- NEW ADDITIONS -----------

// Better ordered struct (less padding)
struct Optimized {
    int i;
    char c1;
    char c2;
};

// Print raw bytes of any object
template <typename T>
void print_bytes(const T& obj) {
    const unsigned char* ptr =
        reinterpret_cast<const unsigned char*>(&obj);

    std::cout << "Bytes: ";
    for (size_t i = 0; i < sizeof(T); ++i) {
        std::cout << std::hex << (int)ptr[i] << " ";
    }
    std::cout << std::dec << "\n";
}

// Print member addresses
void print_addresses(const Packed& p) {
    std::cout << "Addresses (Packed):\n";
    std::cout << "&c1: " << (void*)&p.c1 << "\n";
    std::cout << "&i : " << (void*)&p.i  << "\n";
    std::cout << "&c2: " << (void*)&p.c2 << "\n";
}

// ------------------------------------


int main() {
    std::cout << "Packed size: " << sizeof(Packed) << "\n";
    std::cout << "Packed alignment: " << alignof(Packed) << "\n";
    std::cout << "Offsets in Packed:\n";
    std::cout << "  c1: " << offsetof(Packed, c1) << "\n";
    std::cout << "  i : " << offsetof(Packed, i) << "\n";
    std::cout << "  c2: " << offsetof(Packed, c2) << "\n";

    std::cout << "\nTightlyPacked size: " << sizeof(TightlyPacked) << "\n";
    std::cout << "TightlyPacked alignment: " << alignof(TightlyPacked) << "\n";
    std::cout << "Offsets in TightlyPacked:\n";
    std::cout << "  c1: " << offsetof(TightlyPacked, c1) << "\n";
    std::cout << "  i : " << offsetof(TightlyPacked, i) << "\n";
    std::cout << "  c2: " << offsetof(TightlyPacked, c2) << "\n";

    // -------- NEW FEATURE USAGE --------

    std::cout << "\nOptimized struct size: " << sizeof(Optimized) << "\n";

    // Type trait check
    std::cout << "Packed is standard layout? "
              << std::is_standard_layout<Packed>::value << "\n";

    // Create object and inspect memory
    Packed p{'A', 12345, 'Z'};

    print_addresses(p);
    print_bytes(p);

    std::cout << "\nTightlyPacked bytes:\n";
    TightlyPacked tp{'A', 12345, 'Z'};
    print_bytes(tp);

    // ----------------------------------

    return 0;
}
