#include <ios>
#include <iostream>
#include <istream>
#include <limits>
#include <optional>
#include <print>

unsigned const_number()
{
	return 42;
}

std::optional<int> read_number(std::istream& in)
{
	int result{};
	if (in >> result)
	{
		return result;
	}
	in.clear();
	in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	return {};
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

void guess_number_or_quit(int number)
{
	std::print("Guess the number. \n > ");
	std::optional<int> guess;

	while ((guess = read_number(std::cin)))
	{
		if (guess.value() == number)
		{
			std::println("Well done.");
			return;
		}
		std::print("{} is wrong. Try again. \n> ", guess.value());
	}
	std::println("The number was {}", const_number());
}

int main()
{
	guess_number_or_quit(const_number());

	return 0;
}