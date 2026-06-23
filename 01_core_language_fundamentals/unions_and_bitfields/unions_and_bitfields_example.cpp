#include <iostream>
#include <bitset>
#include <cstdint>
#include <iomanip>
#include <cassert>
#include <cstring>
#include <bit>
#include <array>

union Color {
    std::uint32_t full;
    struct { unsigned char r, g, b, a; } channels;
};

static_assert(sizeof(Color) == sizeof(std::uint32_t));

struct Flags {
    unsigned char a : 1;
    unsigned char b : 1;
    unsigned char c : 1;
    unsigned char d : 1;
    unsigned char e : 4;
};

static_assert(sizeof(Flags) == 1);

void print_color(const Color& c) {
    std::cout << "R=" << static_cast<int>(c.channels.r)
              << " G=" << static_cast<int>(c.channels.g)
              << " B=" << static_cast<int>(c.channels.b)
              << " A=" << static_cast<int>(c.channels.a) << "\n";
}

void print_hex(const Color& c) {
    std::cout << "hex=0x"
              << std::hex << std::setw(8) << std::setfill('0')
              << c.full << std::dec << "\n";
}

void print_bits(std::uint32_t value, const char* label = nullptr) {
    if (label) std::cout << label << ": ";
    std::cout << std::bitset<32>{value} << "\n";
}

void print_channel_bits(const Color& c) {
    std::cout << "R=" << std::bitset<8>{c.channels.r} << "\n"
              << "G=" << std::bitset<8>{c.channels.g} << "\n"
              << "B=" << std::bitset<8>{c.channels.b} << "\n"
              << "A=" << std::bitset<8>{c.channels.a} << "\n";
}

void check_endianness() {
    if constexpr (std::endian::native == std::endian::little)
        std::cout << "Little Endian\n";
    else if constexpr (std::endian::native == std::endian::big)
        std::cout << "Big Endian\n";
    else
        std::cout << "Mixed Endian\n";
}

void print_flag_bits(const Flags& f) {
    unsigned char raw{};
    std::memcpy(&raw, &f, 1);
    std::cout << "Flags raw=" << std::bitset<8>{raw} << "\n";
}

void dump_memory(const void* ptr, std::size_t size) {
    const auto* bytes = static_cast<const unsigned char*>(ptr);
    std::cout << "raw_memory: ";
    for (std::size_t i = 0; i < size; ++i)
        std::cout << std::hex << std::setw(2) << std::setfill('0')
                  << static_cast<int>(bytes[i]) << " ";
    std::cout << std::dec << "\n";
}

void bitwise_demo(std::uint32_t value) {
    print_bits(value,       "original  ");
    print_bits(value << 1,  "shl 1     ");
    print_bits(value >> 1,  "shr 1     ");
    print_bits(~value,      "NOT       ");
}

void memcpy_demo() {
    constexpr std::uint32_t value = 0x12345678;
    std::array<unsigned char, 4> buf{};
    std::memcpy(buf.data(), &value, sizeof(value));
    std::cout << "memcpy bytes: ";
    for (unsigned char b : buf)
        std::cout << std::hex << std::setw(2) << std::setfill('0')
                  << static_cast<int>(b) << " ";
    std::cout << std::dec << "\n";
}

void cast_vs_memcpy_demo() {
    constexpr std::uint32_t value = 0xDEADBEEF;
    const auto* cast_ptr = reinterpret_cast<const unsigned char*>(&value);
    std::array<unsigned char, 4> buf{};
    std::memcpy(buf.data(), &value, sizeof(value));
    std::cout << "reinterpret[0]=0x" << std::hex << static_cast<int>(cast_ptr[0])
              << " memcpy[0]=0x" << static_cast<int>(buf[0]) << std::dec << "\n";
}

[[nodiscard]] constexpr std::uint32_t toggle_bit(std::uint32_t v, int bit) noexcept {
    return v ^ (std::uint32_t{1} << bit);
}

[[nodiscard]] constexpr int count_set_bits(std::uint32_t v) noexcept {
    return std::popcount(v);
}

void print_sizes() {
    std::cout << "sizeof(Color)="    << sizeof(Color)    << "\n"
              << "sizeof(Flags)="    << sizeof(Flags)    << "\n"
              << "sizeof(uint32_t)=" << sizeof(std::uint32_t) << "\n";
}

int main() {
    std::cout << "=== Endianness ===\n";
    check_endianness();

    std::cout << "\n=== Color union ===\n";
    Color c{};
    c.full = 0xAABBCCDD;
    print_color(c);
    print_hex(c);
    print_bits(c.full);

    std::cout << "\n=== Channel bits ===\n";
    print_channel_bits(c);

    std::cout << "\n=== Flags bitfield ===\n";
    Flags f{};
    f.a = 1; f.b = 0; f.c = 1; f.d = 1;
    print_flag_bits(f);

    std::cout << "\n=== Sizes ===\n";
    print_sizes();

    std::cout << "\n=== Memory dump ===\n";
    dump_memory(&c, sizeof(c));

    std::cout << "\n=== Bitwise operations ===\n";
    bitwise_demo(c.full);

    std::cout << "\n=== memcpy ===\n";
    memcpy_demo();

    std::cout << "\n=== reinterpret vs memcpy ===\n";
    cast_vs_memcpy_demo();

    std::cout << "\n=== Bit utilities ===\n";
    std::cout << "set_bits(c.full)=" << count_set_bits(c.full) << "\n";
    const auto toggled = toggle_bit(c.full, 0);
    print_bits(toggled, "after toggle_bit(0)");

    std::cout << "\n=== Addresses ===\n";
    std::cout << "Color addr=" << static_cast<void*>(&c) << "\n"
              << "Flags addr=" << static_cast<void*>(&f) << "\n";

    return 0;
}
