// Unions and Bitfields Exercise
// Memory-efficient data structures

#include <iostream>
#include <cstring>

union Color {
    uint32_t full;
    struct {
        uint8_t r, g, b, a;
    } rgba;
};

struct Flags {
    bool enabled : 1;
    bool readonly : 1;
    bool reserved : 6;
};

int main() {
    Color c;
    c.full = 0xFF0000FF;
    
    Flags f;
    f.enabled = true;
    f.readonly = false;
    
    std::cout << "Union size: " << sizeof(Color) << "\n";
    std::cout << "Bitfield size: " << sizeof(Flags) << "\n";
    
    return 0;
}
