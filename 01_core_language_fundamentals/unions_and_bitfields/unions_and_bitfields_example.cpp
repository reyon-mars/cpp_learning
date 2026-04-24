#include <iostream>   // ✅ ADDED
#include <bitset>
#include <cstdint>    // ✅ ADDED
#include <iomanip>    // ✅ ADDED
#include <cassert>    // ✅ ADDED

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

// ----------------------------------------------

int main() {

    std::cout << "\nAdvanced Memory Concepts:\n";

    // ✅ Endianness
    check_endianness();

    // ✅ ADDED: create color for testing
    Color c;
    c.full = 0xAABBCCDD;

    assert(sizeof(Color) == sizeof(uint32_t));  // ✅ ADDED sanity check

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

    return 0;
}
