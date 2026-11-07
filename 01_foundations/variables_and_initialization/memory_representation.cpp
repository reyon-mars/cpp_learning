#include <array>
#include <bit>
#include <climits>
#include <cstdint>
#include <print>

int main()
{
	static_assert(CHAR_BIT == 8, "A byte on this machine is not 8 bits.");

	auto x{0x12345678U};
	const auto bytes{std::bit_cast<std::array<uint8_t, 4>, uint32_t>(x)};

	std::println("Actual Value: {:#x}", x);

	std::println("Memory Representation of 0x{:X}", x);
	for (const auto& byte : bytes)
	{
		std::println(" At {:#X} : {:#X} ", std::bit_cast<uintptr_t, decltype(&byte)>(&byte), byte);
	}
}