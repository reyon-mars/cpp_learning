#include <cstdlib>
#include <iostream>
#include <print>

int main()
{
	int num1{}, num2{};
	std::println("Please enter two numbers :");
	std::cin >> num1 >> num2;

	std::cout << std::abs(num1) << ' ' << std::abs(num2);
	std::println();
	return 0;
}