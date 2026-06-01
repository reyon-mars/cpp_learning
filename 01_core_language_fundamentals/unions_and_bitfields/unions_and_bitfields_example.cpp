#include <iostream>   // ✅ ADDED
#include <bitset>
#include <cstdint>    // ✅ ADDED
#include <iomanip>    // ✅ ADDED
#include <cassert>    // ✅ ADDED
#include <cstring>    // ✅ NEW

// ✅ ADDED: Color union for reinterpretation
union Color {
    uint32_t full;
    struct {
        unsigned char r, g, b, a;
    } channels;
};

// ✅ ADDED: Print color channels
void print_color(const Color& c) {
    std::cout << "R: " << (int)c.channels.r << " "
              << "G: " << (int)c.channels.g << " "
              << "B: " << (int)c.channels.b << " "
              << "A: " << (int)c.channels.a << "\n";
}

// ✅ ADDED: Print hex value
void print_hex(const Color& c) {
    std::cout << "Hex: 0x"
              << std::hex << std::setw(8) << std::setfill('0')
              << c.full
              << std::dec << "\n";
}

// ✅ ADDED: Bitfield struct
struct Flags {
    unsigned char a : 1;
    unsigned char b : 1;
    unsigned char c : 1;
    unsigned char d : 1;
    unsigned char e : 4; // remaining bits
};

// Detect endianness
void check_endianness() {
    uint32_t value = 0x01020304;
    unsigned char* ptr = reinterpret_cast<unsigned char*>(&value);

    if (ptr[0] == 0x04)
        std::cout << "System is Little Endian\n";
    else
        std::cout << "System is Big Endian\n";
}

// Safer bit representation using std::bitset
void print_bits(uint32_t value) {
    std::bitset<32> bits(value);
    std::cout << "Bits: " << bits << "\n";
}

// Demonstrate union reinterpretation
void reinterpret_demo() {
    Color c;
    c.full = 0x11223344;

    std::cout << "Reinterpret demo:\n";
    print_color(c);
    print_hex(c);
}

// Bitfield raw representation
void print_flag_bits(const Flags& f) {
    const unsigned char* ptr =
        reinterpret_cast<const unsigned char*>(&f);

    std::cout << "Flag raw byte: "
              << std::bitset<8>(*ptr) << "\n";
}

// --------------------------------------------------
// ✅ NEW SMALL ADDITIONS
// --------------------------------------------------

// Print raw memory bytes
void dump_memory(const void* ptr, size_t size) {
    const unsigned char* bytes =
        reinterpret_cast<const unsigned char*>(ptr);

    std::cout << "Raw memory: ";

    for (size_t i = 0; i < size; ++i) {
        std::cout << std::hex
                  << std::setw(2)
                  << std::setfill('0')
                  << (int)bytes[i] << " ";
    }

    std::cout << std::dec << "\n";
}

// Demonstrate bitwise operations
void bitwise_demo(uint32_t value) {

    std::cout << "\nBitwise Operations Demo:\n";

    std::cout << "Original: ";
    print_bits(value);

    std::cout << "Shift left by 1:\n";
    print_bits(value << 1);

    std::cout << "Shift right by 1:\n";
    print_bits(value >> 1);

    std::cout << "Bitwise NOT:\n";
    print_bits(~value);
}

// Show size information
void print_sizes() {
    std::cout << "\nSize Information:\n";

    std::cout << "sizeof(Color): "
              << sizeof(Color) << " bytes\n";

    std::cout << "sizeof(Flags): "
              << sizeof(Flags) << " bytes\n";

    std::cout << "sizeof(uint32_t): "
              << sizeof(uint32_t) << " bytes\n";
}

// Simple memcpy reinterpretation
void memcpy_demo() {

    uint32_t value = 0x12345678;

    unsigned char buffer[4];

    std::memcpy(buffer, &value, sizeof(value));

    std::cout << "\nMemcpy byte copy:\n";

    for (unsigned char b : buffer) {
        std::cout << std::hex
                  << std::setw(2)
                  << std::setfill('0')
                  << (int)b << " ";
    }

    std::cout << std::dec << "\n";
}

// --------------------------------------------------
// ✅ EXTRA ADDITIONS (NEW)
// --------------------------------------------------

// Show binary representation of each color channel
void print_channel_bits(const Color& c) {
    std::cout << "\nChannel Bit Patterns:\n";

    std::cout << "R: "
              << std::bitset<8>(c.channels.r) << "\n";

    std::cout << "G: "
              << std::bitset<8>(c.channels.g) << "\n";

    std::cout << "B: "
              << std::bitset<8>(c.channels.b) << "\n";

    std::cout << "A: "
              << std::bitset<8>(c.channels.a) << "\n";
}

// Toggle a bit in an integer
uint32_t toggle_bit(uint32_t value, int bit) {
    return value ^ (1u << bit);
}

// Count set bits
int count_set_bits(uint32_t value) {
    int count = 0;

    while (value) {
        count += value & 1;
        value >>= 1;
    }

    return count;
}

// Compare reinterpret_cast vs memcpy
void cast_vs_memcpy_demo() {

    uint32_t value = 0xDEADBEEF;

    unsigned char* castPtr =
        reinterpret_cast<unsigned char*>(&value);

    unsigned char buffer[4];
    std::memcpy(buffer, &value, sizeof(value));

    std::cout << "\nCast first byte: "
              << std::hex
              << (int)castPtr[0]
              << std::dec << "\n";

    std::cout << "Memcpy first byte: "
              << std::hex
              << (int)buffer[0]
              << std::dec << "\n";
}

// --------------------------------------------------

int main() {

    std::cout << "\nAdvanced Memory Concepts:\n";

    // ✅ Endianness
    check_endianness();

    // ✅ ADDED: create color for testing
    Color c;
    c.full = 0xAABBCCDD;

    assert(sizeof(Color) == sizeof(uint32_t));

    // ✅ Bit visualization
    print_bits(c.full);

    // ✅ Union reinterpretation
    reinterpret_demo();

    // ✅ ADDED: create flags
    Flags f{};
    f.a = 1;
    f.b = 0;
    f.c = 1;
    f.d = 1;

    // ✅ Bitfield raw view
    print_flag_bits(f);

    // --------------------------------------------------
    // ✅ NEW FEATURE USAGE
    // --------------------------------------------------

    print_sizes();

    std::cout << "\nColor memory dump:\n";
    dump_memory(&c, sizeof(c));

    bitwise_demo(c.full);

    memcpy_demo();

    // ✅ NEW: Channel verification
    std::cout << "\nDirect channel access:\n";
    std::cout << "Red channel: "
              << (int)c.channels.r << "\n";

    std::cout << "Green channel: "
              << (int)c.channels.g << "\n";

    std::cout << "Blue channel: "
              << (int)c.channels.b << "\n";

    std::cout << "Alpha channel: "
              << (int)c.channels.a << "\n";

    // ✅ NEW: Bitfield size check
    static_assert(sizeof(Flags) == 1,
                  "Flags should occupy 1 byte");

    // --------------------------------------------------
    // ✅ EXTRA FEATURE USAGE
    // --------------------------------------------------

    print_channel_bits(c);

    std::cout << "\nSet bits in color value: "
              << count_set_bits(c.full)
              << "\n";

    uint32_t toggled = toggle_bit(c.full, 0);

    std::cout << "\nAfter toggling bit 0:\n";
    print_bits(toggled);

    cast_vs_memcpy_demo();

    std::cout << "\nMemory address of Color object: "
              << &c << "\n";

    std::cout << "Memory address of Flags object: "
              << &f << "\n";

    // --------------------------------------------------

    return 0;
}
