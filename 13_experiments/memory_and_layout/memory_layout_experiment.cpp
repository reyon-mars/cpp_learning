#include <iostream>
#include <cstring>
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <type_traits>
#include <array>
#include <numeric>
#include <algorithm>
#include <string_view>

struct Unaligned   { char c; int i; char d; };
struct Aligned     { int i;  char c; char d; };
struct Optimized   { int i;  char c; char d; };

#pragma pack(push, 1)
struct Packed      { char c; int i; char d; };
#pragma pack(pop)

struct alignas(16) CustomAligned { char c; int i; };

static_assert(alignof(CustomAligned) == 16);
static_assert(sizeof(Packed) < sizeof(Unaligned));
static_assert(sizeof(Optimized) <= sizeof(Unaligned));
static_assert(std::is_trivially_copyable_v<Unaligned>);
static_assert(std::is_trivially_copyable_v<Packed>);

template<typename T>
void print_layout(std::string_view name) {
    std::cout << name << ": sizeof=" << sizeof(T)
              << " alignof=" << alignof(T)
              << " is_trivially_copyable=" << std::boolalpha << std::is_trivially_copyable_v<T> << "\n";
}

template<typename T>
void print_padding(std::string_view name) {
    const std::size_t raw = sizeof(char) + sizeof(int) + sizeof(char);
    std::cout << name << " padding=" << (sizeof(T) - raw) << " bytes\n";
}

void dump_bytes(const void* ptr, std::size_t size) {
    const auto* bytes = static_cast<const unsigned char*>(ptr);
    for (std::size_t i = 0; i < size; ++i)
        std::cout << std::setw(2) << std::setfill('0') << std::hex
                  << static_cast<int>(bytes[i]) << " ";
    std::cout << std::dec << "\n";
}

void show_member_spacing() {
    Unaligned obj{};
    const auto c_addr = reinterpret_cast<const char*>(&obj.c);
    const auto i_addr = reinterpret_cast<const char*>(&obj.i);
    const auto d_addr = reinterpret_cast<const char*>(&obj.d);
    std::cout << "c→i=" << (i_addr - c_addr) << " bytes  "
              << "i→d=" << (d_addr - i_addr) << " bytes\n";
}

void show_sizes() {
    constexpr std::array<std::size_t, 5> sizes{
        sizeof(Unaligned), sizeof(Aligned), sizeof(Packed),
        sizeof(CustomAligned), sizeof(Optimized)
    };
    const auto total = std::accumulate(sizes.begin(), sizes.end(), std::size_t{0});
    const auto max   = *std::ranges::max_element(sizes);
    std::cout << "combined=" << total << " largest=" << max << " bytes\n";
}

int main() {
    std::cout << "=== Sizes ===\n";
    print_layout<Unaligned>("Unaligned");
    print_layout<Aligned>("Aligned");
    print_layout<Packed>("Packed");
    print_layout<CustomAligned>("CustomAligned");
    print_layout<Optimized>("Optimized");

    std::cout << "\n=== Padding (char+int+char structs) ===\n";
    print_padding<Unaligned>("Unaligned");
    print_padding<Aligned>("Aligned");
    print_padding<Packed>("Packed");

    std::cout << "\n=== Offsets (Unaligned) ===\n";
    std::cout << "c=" << offsetof(Unaligned, c)
              << " i=" << offsetof(Unaligned, i)
              << " d=" << offsetof(Unaligned, d) << "\n";

    std::cout << "\n=== Member spacing (Unaligned) ===\n";
    show_member_spacing();

    std::cout << "\n=== Addresses (Unaligned) ===\n";
    Unaligned obj{};
    std::cout << "&obj="   << static_cast<void*>(&obj)   << "\n"
              << "&obj.c=" << static_cast<void*>(&obj.c) << "\n"
              << "&obj.i=" << static_cast<void*>(&obj.i) << "\n"
              << "&obj.d=" << static_cast<void*>(&obj.d) << "\n";

    std::cout << "\n=== Memory dump (zeroed Unaligned) ===\n";
    dump_bytes(&obj, sizeof(obj));

    std::cout << "\n=== Filled memory dump ===\n";
    obj.c = 'A'; obj.i = 123456; obj.d = 'Z';
    dump_bytes(&obj, sizeof(obj));

    std::cout << "\n=== Combined stats ===\n";
    show_sizes();

    std::cout << "\n=== Comparisons ===\n";
    std::cout << std::boolalpha
              << "Packed < Unaligned="     << (sizeof(Packed)     < sizeof(Unaligned)) << "\n"
              << "Optimized <= Unaligned=" << (sizeof(Optimized) <= sizeof(Unaligned)) << "\n";

    return 0;
}
