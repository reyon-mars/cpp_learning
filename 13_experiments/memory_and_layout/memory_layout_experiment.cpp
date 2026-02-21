// Memory and Layout Experiment
// Understanding object layout, padding, and alignment

#include <iostream>
#include <cstring>

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

int main() {
    std::cout << "Size of Unaligned: " << sizeof(Unaligned) << "\n";
    std::cout << "Size of Aligned: " << sizeof(Aligned) << "\n";
    
    std::cout << "Offset in Unaligned:\n";
    std::cout << "  c: " << offsetof(Unaligned, c) << "\n";
    std::cout << "  i: " << offsetof(Unaligned, i) << "\n";
    std::cout << "  d: " << offsetof(Unaligned, d) << "\n";
    
    return 0;
}
