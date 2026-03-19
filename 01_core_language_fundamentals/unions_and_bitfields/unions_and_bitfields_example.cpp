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

// ----------- NEW ADDITIONS -----------

// Set RGBA values manually
void set_color(Color& c, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    c.rgba.r = r;
    c.rgba.g = g;
    c.rgba.b = b;
    c.rgba.a = a;
}

// Print packed hex value
void print_hex(const Color& c) {
    std::cout << "Packed value: 0x"
              << std::hex << c.full << std::dec << "\n";
}

// Reset all flags
void reset_flags(Flags& f) {
    f.enabled = false;
    f.readonly = false;
    f.reserved = 0;
}

// Count active flags
int count_flags(const Flags& f) {
    return f.enabled + f.readonly;
}

// Show raw memory bytes
void print_bytes(const Color& c) {
    const unsigned char* bytes =
        reinterpret_cast<const unsigned char*>(&c);

    std::cout << "Raw bytes: ";
    for (size_t i = 0; i < sizeof(Color); ++i) {
        std::cout << std::hex << static_cast<int>(bytes[i]) << " ";
    }
    std::cout << std::dec << "\n";
}

// ------------------------------------

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

    // -------- NEW FEATURE USAGE --------

    print_hex(c);
    print_bytes(c);

    set_color(c, 10, 20, 30, 255);
    std::cout << "\nAfter setting new color:\n";
    print_color(c);
    print_hex(c);

    std::cout << "Active flags count: "
              << count_flags(f) << "\n";

    reset_flags(f);
    std::cout << "After reset:\n";
    print_flags(f);

    // ----------------------------------

    return 0;
}
