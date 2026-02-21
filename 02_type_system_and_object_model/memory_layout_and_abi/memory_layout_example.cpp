// Memory Layout and ABI Exercise
// Exploring object layout and padding

#include <iostream>

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

int main() {
    std::cout << "Packed size: " << sizeof(Packed) << "\n";
    std::cout << "Offsets in Packed:\n";
    std::cout << "  c1: " << offsetof(Packed, c1) << "\n";
    std::cout << "  i: " << offsetof(Packed, i) << "\n";
    std::cout << "  c2: " << offsetof(Packed, c2) << "\n";
    
    std::cout << "\nTightlyPacked size: " << sizeof(TightlyPacked) << "\n";
    
    return 0;
}
