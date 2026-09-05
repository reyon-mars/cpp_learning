#include <print>

int main()
{

	unsigned u{10}, u2{42};

	std::println("Unsigned \n {}-{} : {} ", u, u2, (u - u2));
	std::println("{}-{}: ", u2, u, (u2 - u));

	int i{10}, i2{42};

	std::println("Signed \n {}-{} : {}", i, i2, (i - i2));
	std::println("{}-{} : {}", i2, i, (i2 - i));

	std::println("Mixed \n {} - {} : {} ", i, u, (i - u));
	std::println("{}-{} : {}", u, i, (u - i));

	return 0;
}