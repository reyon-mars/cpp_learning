#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <string_view>
#include <type_traits>

#pragma pack(push, 1)
struct Packed
{
	char c;
	int i;
	char d;
};
#pragma pack(pop)

struct Optimized
{
	int i;
	char c;
	char d;
};

struct Unaligned
{
	char a;
	int b;
	char c;
};

struct Compact
{
	double value;
	int id;
	char flag;
};

struct alignas(16) Aligned16
{
	int x;
	double y;
};

static_assert(alignof(Aligned16) == 16);
static_assert(sizeof(Packed) < sizeof(Unaligned));
static_assert(std::is_trivially_copyable_v<Packed>);
static_assert(std::is_standard_layout_v<Optimized>);
static_assert(sizeof(Compact) >= sizeof(double));

template <typename T>
void print_layout(std::string_view name)
{
	std::cout << name << ": sizeof=" << sizeof(T) << " alignof=" << alignof(T)
			  << " padding=" << (sizeof(T) % alignof(T)) << "\n";
}

void print_offsets_unaligned()
{
	std::cout << "Unaligned offsets:"
			  << " a=" << offsetof(Unaligned, a) << " b=" << offsetof(Unaligned, b) << " c=" << offsetof(Unaligned, c)
			  << "\n";
}

void dump_bytes(const void* ptr, std::size_t size)
{
	const auto* bytes = static_cast<const unsigned char*>(ptr);
	for (std::size_t i = 0; i < size; ++i)
		std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(bytes[i]) << " ";
	std::cout << std::dec << "\n";
}

void print_addresses(const Packed& p)
{
	std::cout << "Packed member addresses:"
			  << " c=" << static_cast<const void*>(&p.c) << " i=" << static_cast<const void*>(&p.i)
			  << " d=" << static_cast<const void*>(&p.d) << "\n";
}

int main()
{
	std::cout << "=== Layout comparison ===\n";
	print_layout<Packed>("Packed");
	print_layout<Unaligned>("Unaligned");
	print_layout<Optimized>("Optimized");
	print_layout<Compact>("Compact");
	print_layout<Aligned16>("Aligned16");

	std::cout << "\n=== Padding (Packed only, no internal padding) ===\n";
	std::cout << "Packed overhead vs raw sum=" << sizeof(Packed) - (sizeof(char) + sizeof(int) + sizeof(char)) << "\n";

	std::cout << "\n=== Offsets ===\n";
	print_offsets_unaligned();

	std::cout << "\n=== Type traits ===\n";
	std::cout << std::boolalpha << "Packed trivially_copyable=" << std::is_trivially_copyable_v<Packed> << "\n"
			  << "Optimized standard_layout=" << std::is_standard_layout_v<Optimized> << "\n"
			  << "Packed is_trivial=" << std::is_trivial_v<Packed> << "\n";

	std::cout << "\n=== Aligned16 ===\n";
	Aligned16 a16{1, 2.5};
	std::cout << "address=0x" << std::hex << reinterpret_cast<std::uintptr_t>(&a16) << std::dec << "\n"
			  << "16-byte aligned=" << std::boolalpha << (reinterpret_cast<std::uintptr_t>(&a16) % 16 == 0) << "\n";

	std::cout << "\n=== Packed memory dump ===\n";
	Packed p{'A', 12345, 'Z'};
	dump_bytes(&p, sizeof(p));
	print_addresses(p);

	std::cout << "\n=== Compact ===\n";
	Compact comp{3.14, 101, 'Y'};
	std::cout << "value=" << comp.value << " id=" << comp.id << " flag=" << comp.flag << "\n";

	return 0;
}
