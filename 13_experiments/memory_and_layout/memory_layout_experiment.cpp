// Memory and Layout Experiment
// Understanding object layout, padding, and alignment

#include <iostream>
#include <cstring>
#include <cstddef>

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

int main() {

    std::cout << "Size of Unaligned: " << sizeof(Unaligned) << "\n";
    std::cout << "Size of Aligned: " << sizeof(Aligned) << "\n";
    std::cout << "Size of Packed: " << sizeof(Packed) << "\n";

    std::cout << "\nAlignment requirements:\n";
    std::cout << "alignof(Unaligned): " << alignof(Unaligned) << "\n";
    std::cout << "alignof(Aligned): " << alignof(Aligned) << "\n";

    std::cout << "\nOffset in Unaligned:\n";
    std::cout << "  c: " << offsetof(Unaligned, c) << "\n";
    std::cout << "  i: " << offsetof(Unaligned, i) << "\n";
    std::cout << "  d: " << offsetof(Unaligned, d) << "\n";

    // Small memory layout visualization
    Unaligned obj{};
    std::cout << "\nMemory addresses:\n";
    std::cout << "  &obj: " << &obj << "\n";
    std::cout << "  &obj.c: " << static_cast<void*>(&obj.c) << "\n";
    std::cout << "  &obj.i: " << static_cast<void*>(&obj.i) << "\n";
    std::cout << "  &obj.d: " << static_cast<void*>(&obj.d) << "\n";

    return 0;
}
