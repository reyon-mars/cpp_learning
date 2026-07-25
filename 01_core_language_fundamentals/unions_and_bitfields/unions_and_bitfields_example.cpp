#include <array>
#include <bit>
#include <bitset>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <format>
#include <ranges>
#include <span>
#include <string_view>
#include <concepts>
#include <type_traits>

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
    std::cout << std::format("R={} G={} B={} A={}\n",
                             c.channels.r, c.channels.g, c.channels.b, c.channels.a);
}

void print_hex(const Color& c) {
    std::cout << std::format("hex=0x{:08X}\n", c.full);
}

void print_bits(std::uint32_t value, std::string_view label = "") {
    if (!label.empty()) std::cout << std::format("{}: ", label);
    std::cout << std::bitset<32>{value} << '\n';
}

void print_channel_bits(const Color& c) {
    std::cout << std::format("R={}\nG={}\nB={}\nA={}\n",
                             std::bitset<8>{c.channels.r}.to_string(),
                             std::bitset<8>{c.channels.g}.to_string(),
                             std::bitset<8>{c.channels.b}.to_string(),
                             std::bitset<8>{c.channels.a}.to_string());
}

void check_endianness() {
    if constexpr      (std::endian::native == std::endian::little) std::cout << "Little Endian\n";
    else if constexpr (std::endian::native == std::endian::big)    std::cout << "Big Endian\n";
    else                                                            std::cout << "Mixed Endian\n";
}

void print_flag_bits(const Flags& f) {
    unsigned char raw{};
    std::memcpy(&raw, &f, 1);
    std::cout << std::format("Flags raw={}\n", std::bitset<8>{raw}.to_string());
}

void dump_memory(const void* ptr, std::size_t size) {
    const auto* bytes = static_cast<const unsigned char*>(ptr);
    std::cout << "raw_memory: ";
    for (std::size_t i = 0; i < size; ++i)
        std::cout << std::format("{:02X} ", bytes[i]);
    std::cout << '\n';
}

void bitwise_demo(std::uint32_t value) {
    print_bits(value,         "original  ");
    print_bits(value << 1,    "shl 1     ");
    print_bits(value >> 1,    "shr 1     ");
    print_bits(~value,        "NOT       ");
    print_bits(value & 0xFF,  "AND 0xFF  ");
    print_bits(value | 0xFF,  "OR  0xFF  ");
    print_bits(value ^ 0xFFFFFFFF, "XOR ~0");
}

void memcpy_demo() {
    constexpr std::uint32_t value = 0x12345678;
    std::array<unsigned char, 4> buf{};
    std::memcpy(buf.data(), &value, sizeof(value));
    std::cout << "memcpy bytes: ";
    for (unsigned char b : buf) std::cout << std::format("{:02X} ", b);
    std::cout << '\n';
}

void cast_vs_memcpy_demo() {
    constexpr std::uint32_t value = 0xDEADBEEF;
    const auto* cast_ptr = reinterpret_cast<const unsigned char*>(&value);
    std::array<unsigned char, 4> buf{};
    std::memcpy(buf.data(), &value, sizeof(value));
    std::cout << std::format("reinterpret[0]=0x{:02X} memcpy[0]=0x{:02X}\n",
                             cast_ptr[0], buf[0]);
}

void bit_cast_demo() {
    constexpr float  f_val  = 1.0f;
    constexpr auto   bits   = std::bit_cast<std::uint32_t>(f_val);
    constexpr float  back   = std::bit_cast<float>(bits);

    std::cout << std::format("bit_cast float→u32: {:08X}\n", bits);
    std::cout << std::format("bit_cast u32→float: {}\n", back);
    static_assert(back == f_val);
}

[[nodiscard]] constexpr std::uint32_t toggle_bit(std::uint32_t v, int bit) noexcept {
    return v ^ (std::uint32_t{1} << bit);
}

[[nodiscard]] constexpr std::uint32_t set_bit  (std::uint32_t v, int bit) noexcept {
    return v | (std::uint32_t{1} << bit);
}

[[nodiscard]] constexpr std::uint32_t clear_bit(std::uint32_t v, int bit) noexcept {
    return v & ~(std::uint32_t{1} << bit);
}

[[nodiscard]] constexpr bool test_bit(std::uint32_t v, int bit) noexcept {
    return (v >> bit) & 1u;
}

[[nodiscard]] constexpr int count_set_bits(std::uint32_t v) noexcept {
    return std::popcount(v);
}

[[nodiscard]] constexpr int leading_zeros(std::uint32_t v) noexcept {
    return std::countl_zero(v);
}

[[nodiscard]] constexpr int trailing_zeros(std::uint32_t v) noexcept {
    return std::countr_zero(v);
}

[[nodiscard]] constexpr std::uint32_t byteswap(std::uint32_t v) noexcept {
    return std::byteswap(v);
}

[[nodiscard]] constexpr std::uint32_t rotate_left(std::uint32_t v, int n) noexcept {
    return std::rotl(v, n);
}

[[nodiscard]] constexpr std::uint32_t rotate_right(std::uint32_t v, int n) noexcept {
    return std::rotr(v, n);
}

void print_sizes() {
    std::cout << std::format("sizeof(Color)    = {}\n"
                             "sizeof(Flags)    = {}\n"
                             "sizeof(uint32_t) = {}\n",
                             sizeof(Color), sizeof(Flags), sizeof(std::uint32_t));
}

void bit_ops_summary(std::uint32_t v) {
    std::cout << std::format("value:           {:08X}\n"
                             "popcount:        {}\n"
                             "leading_zeros:   {}\n"
                             "trailing_zeros:  {}\n"
                             "byteswap:        {:08X}\n"
                             "rotl(4):         {:08X}\n"
                             "rotr(4):         {:08X}\n",
                             v,
                             count_set_bits(v),
                             leading_zeros(v),
                             trailing_zeros(v),
                             byteswap(v),
                             rotate_left(v, 4),
                             rotate_right(v, 4));
}

static_assert(toggle_bit(0b1010u, 0) == 0b1011u);
static_assert(set_bit   (0b1010u, 0) == 0b1011u);
static_assert(clear_bit (0b1011u, 0) == 0b1010u);
static_assert(test_bit  (0b1010u, 1));
static_assert(!test_bit (0b1010u, 0));
static_assert(count_set_bits(0b1111u) == 4);
static_assert(byteswap(std::uint32_t{0x12345678}) == 0x78563412u);
static_assert(rotate_left(std::uint32_t{1}, 4) == 16u);

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

    std::cout << "\n=== std::bit_cast ===\n";
    bit_cast_demo();

    std::cout << "\n=== Bit utilities ===\n";
    bit_ops_summary(c.full);

    const auto toggled = toggle_bit(c.full, 0);
    print_bits(toggled, "toggle_bit(0)");

    const auto set = set_bit(c.full, 2);
    print_bits(set, "set_bit(2)   ");

    const auto cleared = clear_bit(c.full, 0);
    print_bits(cleared, "clear_bit(0) ");

    std::cout << std::format("test_bit(c.full, 0)={}\n", test_bit(c.full, 0) ? "1" : "0");

    std::cout << "\n=== Bit iteration via views ===\n";
    std::cout << "Set bit indices in c.full: ";
    for (int i : std::views::iota(0, 32))
        if (test_bit(c.full, i)) std::cout << std::format("{} ", i);
    std::cout << '\n';

    std::cout << "\n=== Addresses ===\n";
    std::cout << std::format("Color addr={}\n", static_cast<void*>(&c));
    std::cout << std::format("Flags addr={}\n", static_cast<void*>(&f));

    std::cout << "\nAll static assertions passed.\n";
    return 0;
}
