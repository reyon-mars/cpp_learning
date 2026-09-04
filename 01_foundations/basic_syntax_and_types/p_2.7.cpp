#include <iostream>
#include <print>

int main()
{

	int input{};

	std::print("Enter 3 digit positive number: ");
	std::cin >> input;

	int sum = input % 10;

	input /= 10;
	sum += (input % 10);

	input /= 10;
	sum += input;

	std::println("Sum of digits: {}", sum);

	return 0;
}