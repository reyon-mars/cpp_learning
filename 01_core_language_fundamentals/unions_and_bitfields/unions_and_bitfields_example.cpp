// Unions and Bitfields Exercise
// Memory-efficient data structures

#include <iostream>
#include <cstdint>
#include <cstring>

// ---------------- ORIGINAL CODE ----------------

union Color {
    uint32_t full;
    struct {
        uint8_t r, g, b, a;
    } rgba;
};

struct Flags {
    bool enabled  : 1;
    bool readonly : 1;
    bool reserved : 6;
};

// ---------------- SMALL ADDITIONS ----------------

// Print color components
void print_color(const Color& c) {
    std::cout << "RGBA = ("
              << static_cast<int>(c.rgba.r) << ", "
              << static_cast<int>(c.rgba.g) << ", "
              << static_cast<int>(c.rgba.b) << ", "
              << static_cast<int>(c.rgba.a) << ")\n";
}

// Toggle enabled flag
void toggle_enabled(Flags& f) {
    f.enabled = !f.enabled;
}

// Print flags state
void print_flags(const Flags& f) {
    std::cout << "Enabled: "  << f.enabled
              << ", Readonly: " << f.readonly << "\n";
}

// ---------------- MAIN ----------------

int main() {
    Color c;
    c.full = 0xFF0000FF;   // packed RGBA

    Flags f{};
    f.enabled = true;
    f.readonly = false;

    std::cout << "Union size: " << sizeof(Color) << "\n";
    std::cout << "Bitfield size: " << sizeof(Flags) << "\n";

    // ---- small added usage ----
    print_color(c);

    print_flags(f);
    toggle_enabled(f);
    std::cout << "After toggle:\n";
    print_flags(f);
    // --------------------------

    return 0;
}
