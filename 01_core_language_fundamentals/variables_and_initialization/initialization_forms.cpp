#include <array>
#include <bit>
#include <bitset>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iomanip>
#include <iostream>

union Color
{
	std::uint32_t full;
	struct
	{
		unsigned char r, g, b, a;
	} channels;
};

static_assert(sizeof(Color) == sizeof(std::uint32_t));

struct Flags
{
	unsigned char a : 1;
	unsigned char b : 1;
	unsigned char c : 1;
	unsigned char d : 1;
	unsigned char e : 4;
};

static_assert(sizeof(Flags) == 1);

void print_color(const Color& c)
{
	std::cout << "R=" << static_cast<int>(c.channels.r) << " G=" << static_cast<int>(c.channels.g)
			  << " B=" << static_cast<int>(c.channels.b) << " A=" << static_cast<int>(c.channels.a) << "\n";
}

void print_hex(const Color& c)
{
	std::cout << "hex=0x" << std::hex << std::setw(8) << std::setfill('0') << c.full << std::dec << "\n";
}

void print_bits(std::uint32_t value, const char* label = nullptr)
{
	if (label)
		std::cout << label << ": ";
	std::cout << std::bitset<32>{value} << "\n";
}

void print_channel_bits(const Color& c)
{
	std::cout << "R=" << std::bitset<8>{c.channels.r} << "\n"
			  << "G=" << std::bitset<8>{c.channels.g} << "\n"
			  << "B=" << std::bitset<8>{c.channels.b} << "\n"
			  << "A=" << std::bitset<8>{c.channels.a} << "\n";
}

void check_endianness()
{
	if constexpr (std::endian::native == std::endian::little)
		std::cout << "Little Endian\n";
	else if constexpr (std::endian::native == std::endian::big)
		std::cout << "Big Endian\n";
	else
		std::cout << "Mixed Endian\n";
}

void print_flag_bits(const Flags& f)
{
	unsigned char raw{};
	std::memcpy(&raw, &f, 1);
	std::cout << "Flags raw=" << std::bitset<8>{raw} << "\n";
}

void dump_memory(const void* ptr, std::size_t size)
{
	const auto* bytes = static_cast<const unsigned char*>(ptr);
	std::cout << "raw_memory: ";
	for (std::size_t i = 0; i < size; ++i)
		std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(bytes[i]) << " ";
	std::cout << std::dec << "\n";
}

void bitwise_demo(std::uint32_t value)
{
	print_bits(value, "original  ");
	print_bits(value << 1, "shl 1     ");
	print_bits(value >> 1, "shr 1     ");
	print_bits(~value, "NOT       ");
}

void memcpy_demo()
{
	constexpr std::uint32_t value = 0x12345678;
	std::array<unsigned char, 4> buf{};
	std::memcpy(buf.data(), &value, sizeof(value));
	std::cout << "memcpy bytes: ";
	for (unsigned char b : buf)
		std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(b) << " ";
	std::cout << std::dec << "\n";
}

void cast_vs_memcpy_demo()
{
	constexpr std::uint32_t value = 0xDEADBEEF;
	const auto* cast_ptr = reinterpret_cast<const unsigned char*>(&value);
	std::array<unsigned char, 4> buf{};
	std::memcpy(buf.data(), &value, sizeof(value));
	std::cout << "reinterpret[0]=0x" << std::hex << static_cast<int>(cast_ptr[0]) << " memcpy[0]=0x"
			  << static_cast<int>(buf[0]) << std::dec << "\n";
}

void bit_cast_demo()
{
	constexpr std::uint32_t packed = 0x11223344;
	const Color safely_punned = std::bit_cast<Color>(packed);
	std::cout << "bit_cast into Color: ";
	print_color(safely_punned);
	const auto round_tripped = std::bit_cast<std::uint32_t>(safely_punned);
	std::cout << "bit_cast round-trip matches original? " << std::boolalpha << (round_tripped == packed) << "\n";
}

[[nodiscard]] constexpr std::uint32_t toggle_bit(std::uint32_t v, int bit) noexcept
{
	return v ^ (std::uint32_t{1} << bit);
}

[[nodiscard]] constexpr int count_set_bits(std::uint32_t v) noexcept
{
	return std::popcount(v);
}

void bit_query_demo(std::uint32_t value)
{
	std::cout << "value=" << value << "\n"
			  << "has_single_bit=" << std::boolalpha << std::has_single_bit(value) << "\n"
			  << "bit_width=" << std::bit_width(value) << "\n"
			  << "bit_ceil=" << std::bit_ceil(value) << "\n"
			  << "bit_floor=" << std::bit_floor(value) << "\n"
			  << "countl_zero=" << std::countl_zero(value) << "\n"
			  << "countr_zero=" << std::countr_zero(value) << "\n"
			  << "countl_one=" << std::countl_one(value) << "\n"
			  << "countr_one=" << std::countr_one(value) << "\n";
}

void rotate_demo(std::uint32_t value)
{
	print_bits(value, "original    ");
	print_bits(std::rotl(value, 8), "rotl 8      ");
	print_bits(std::rotr(value, 8), "rotr 8      ");
}

void byteswap_demo(std::uint32_t value)
{
	print_hex(Color{.full = value});
	const auto swapped = std::byteswap(value);
	std::cout << "byteswapped=0x" << std::hex << std::setw(8) << std::setfill('0') << swapped << std::dec << "\n";
}

void print_sizes()
{
	std::cout << "sizeof(Color)=" << sizeof(Color) << "\n"
			  << "sizeof(Flags)=" << sizeof(Flags) << "\n"
			  << "sizeof(uint32_t)=" << sizeof(std::uint32_t) << "\n";
}

void bitset_operations_demo()
{
	std::bitset<8> bits{0b1010'1010};
	std::cout << "initial=" << bits << " count=" << bits.count()
			  << " any=" << std::boolalpha << bits.any() << " all=" << bits.all() << " none=" << bits.none() << "\n";
	bits.flip();
	std::cout << "after flip=" << bits << "\n";
	bits.set(0, true);
	bits.reset(1);
	std::cout << "after set(0)/reset(1)=" << bits << "\n";
	std::cout << "test(0)=" << bits.test(0) << " to_ulong=" << bits.to_ulong() << " to_string=" << bits.to_string()
			  << "\n";
}

void byte_type_demo()
{
	std::byte b{0b0000'1111};
	const auto shifted = b << 2;
	const auto combined = b | std::byte{0b1111'0000};
	std::cout << "byte to_integer=" << std::to_integer<int>(b) << "\n"
			  << "shifted to_integer=" << std::to_integer<int>(shifted) << "\n"
			  << "combined to_integer=" << std::to_integer<int>(combined) << "\n";
}

int main()
{
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
	f.a = 1;
	f.b = 0;
	f.c = 1;
	f.d = 1;
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

	std::cout << "\n=== std::bit_cast (safe, defined-behavior type punning) ===\n";
	bit_cast_demo();

	std::cout << "\n=== Bit utilities ===\n";
	std::cout << "set_bits(c.full)=" << count_set_bits(c.full) << "\n";
	const auto toggled = toggle_bit(c.full, 0);
	print_bits(toggled, "after toggle_bit(0)");

	std::cout << "\n=== Bit queries (has_single_bit / bit_width / bit_ceil / bit_floor / countl / countr) ===\n";
	bit_query_demo(0b0001'1000);

	std::cout << "\n=== Rotate left / right ===\n";
	rotate_demo(c.full);

	std::cout << "\n=== Byteswap ===\n";
	byteswap_demo(c.full);

	std::cout << "\n=== bitset operations ===\n";
	bitset_operations_demo();

	std::cout << "\n=== std::byte ===\n";
	byte_type_demo();

	std::cout << "\n=== Addresses ===\n";
	std::cout << "Color addr=" << static_cast<void*>(&c) << "\n"
			  << "Flags addr=" << static_cast<void*>(&f) << "\n";

	return 0;
}
