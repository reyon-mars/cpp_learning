#include <iostream>
#include <print>

int main()
{
	int num1{}, num2{};
	std::println("Please enter two numbers: ");
	std::cin >> num1 >> num2;

	std::println(" {} ", (num1 - (num1 / num2) * num2));
	return 0;
}