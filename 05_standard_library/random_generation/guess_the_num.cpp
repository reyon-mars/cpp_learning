#include <ios>
#include <iostream>
#include <limits>
#include <print>

unsigned const_number()
{
	return 42;
}

unsigned input()
{
	unsigned number{};

	while (!(std::cin >> number))
	{
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::println("Please enter a number.");
		std::print("> ");
	}
	return number;
}

void guess_number(unsigned number)
{
	std::print("Guess the number. \n> ");
	unsigned guess = input();

	while (guess != number)
	{
		std::println("{} is wrong. Try again", guess);
		std::print("> ");
		guess = input();
	}

	std::println("Well done.");
}

int main()
{
	guess_number(const_number());

	return 0;
}