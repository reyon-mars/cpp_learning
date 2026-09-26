#include <cstddef>
#include <format>
#include <functional>
#include <ios>
#include <iostream>
#include <istream>
#include <limits>
#include <optional>
#include <print>
#include <random>
#include <string>
#include <vector>

unsigned const_number()
{
	return 42;
}

std::vector<bool> generate_primes(int bound)
{
	std::vector<bool> primes{};
	primes.reserve(bound + 1);
	primes.push_back(false);
	primes.push_back(false);

	bool is_prime{true};
	for (size_t i = 2; i <= bound; ++i)
	{
		is_prime = true;
		for (size_t j = 2; j * j <= i; j++)
		{
			if (i % j == 0)
			{
				is_prime = false;
				break;
			}
		}
		primes.push_back(is_prime);
	}
	return primes;
}

const std::vector<bool> primes = generate_primes(99'999);

inline unsigned generate_random_number(unsigned lower_bound, unsigned upper_bound)
{
	thread_local static std::mt19937 random_engine(std::random_device{}());
	std::uniform_int_distribution<unsigned int> dist(lower_bound, upper_bound);
	return dist(random_engine);
}

unsigned generate_random_prime(unsigned lower_bound, unsigned upper_bound)
{
	if (primes.empty() || primes.size() <= upper_bound)
		return 0;

	auto rand_num{generate_random_number(lower_bound, upper_bound)};

	while (!(primes[rand_num]))
		rand_num = generate_random_number(lower_bound, upper_bound);

	return rand_num;
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

std::string rank_similarity(int number, int guess)
{
	auto number_str = std::format("{:0>5}", (number));
	auto guess_str = std::format("{:0>5}", guess);

	std::string matches(5, '.');
	for (size_t i = 0, end = number_str.size(); i < end; i++)
	{
		char guess_char = guess_str[i];

		if (i < number_str.size() && number_str[i] == guess_char)
		{
			matches[i] = '*';
		}
	}
	return matches;
}

void guess_number_or_quit(int number, std::function<std::string(int, int)> message)
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
		std::print("{}", message(number, guess.value()));
		std::print("> ");
	}
	std::println("The number was {}", const_number());
}

int main()
{
	auto make_message = [](int number, int guess)
	{
		return std::format("Your guess was too {}\n", number < guess ? "big" : "small");
	};
	guess_number_or_quit(generate_random_prime(1, 10'000), make_message);

	return 0;
}