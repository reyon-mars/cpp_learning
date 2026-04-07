// ----------- MORE ADVANCED ADDITIONS -----------

#include <bitset>

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


// ================= ADD IN MAIN =================

// (Add near the end before return)

std::cout << "\nAdvanced Memory Concepts:\n";

// ✅ Endianness
check_endianness();

// ✅ Bit visualization
print_bits(c.full);

// ✅ Union reinterpretation
reinterpret_demo();

// ✅ Bitfield raw view
print_flag_bits(f);
