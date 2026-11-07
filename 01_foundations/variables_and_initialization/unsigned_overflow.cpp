#include <cstdint>
#include <print>

int main()
{
	std::uint8_t x{255};
	std::uint8_t y{1};

	std::println("{} + {} = {}", x, 1, static_cast<std::uint8_t>(x + 1));
	std::println("{} + {} = {}", x, 10, static_cast<std::uint8_t>(x + 10));

	// what_type becomes int due to integer promotion.
	// integer promotion happens when arithmetic is performed on integral types
	// and int can fit it.
	auto what_type{x + y};
	std::println("Integral promotion");
	std::println("{}", typeid(what_type).name());

	return 0;
}