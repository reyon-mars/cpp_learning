#include <bitset>
#include <cstdint>
#include <print>

int main()
{
	const std::uint8_t positive{13};
	const std::int8_t negative{-13};

	std::println("Binary Representation of unsigned {}", positive);
	std::println("{}", std::bitset<8>(positive).to_string());

	std::println("Binary Representation of signed {}", negative);
	std::println("{}", std::bitset<8>(static_cast<uint8_t>(negative)).to_string());

	return 0;
}