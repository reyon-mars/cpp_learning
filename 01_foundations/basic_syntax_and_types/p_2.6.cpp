#include <iostream>
#include <print>

int main()
{
	float num1{}, num2{};

	std::print("Enter the first number: ");
	std::cin >> num1;
	std::println();
	std::print("Enter the second number: ");
	std::cin >> num2;
	std::println();

	float div_result = num1 / num2;

	std::println("Integer Part: {}", static_cast<int>(div_result));
	std::println("Fractional Part: {}", (num1 - (num2 * static_cast<int>(div_result))));

	return 0;
}