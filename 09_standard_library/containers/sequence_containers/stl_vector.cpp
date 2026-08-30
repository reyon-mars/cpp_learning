#include <algorithm>
#include <format>
#include <iostream>
#include <iterator>
#include <ostream>
#include <print>
#include <string>
#include <vector>

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<std::vector<T>>& triangle)
{
	for (const auto& row : triangle)
	{
		std::ranges::copy(row, std::ostream_iterator<T>(os, " "));
		os << '\n';
	}
	return os;
}

std::vector<int> get_next_row(const std::vector<int>& prev_row)
{
	std::vector next_row{1};

	if (prev_row.empty())
	{
		return next_row;
	}

	next_row.reserve(prev_row.size() + 2);

	for (std::size_t idx{0}; idx < prev_row.size() - 1; ++idx)
	{
		next_row.emplace_back(prev_row[idx] + prev_row[idx + 1]);
	}

	next_row.emplace_back(1);

	return next_row;
}

auto generate_triangle(int num_rows)
{
	std::vector<std::vector<int>> triangle{{1}};
	triangle.reserve(num_rows);

	for (int row = 1; row < num_rows; ++row)
	{
		triangle.push_back(get_next_row(triangle.back()));
	}

	return triangle;
}

void show_vectors(std::ostream& os, std::vector<std::vector<int>>& v)
{
	size_t base_width = v.back().size();

	std::string spaces(base_width * 3, ' ');

	for (const auto& row : v)
	{
		os << spaces;
		if (spaces.size() > 3)
		{
			spaces.resize(spaces.size() - 3);
		}
		for (const auto& data : row)
		{
			os << std::format("{: ^{}}", data, 6);
		}
		os << '\n';
	}
}

int main()
{
	std::println("Please enter the number of rows: ");
	int num_rows{};

	std::cin >> num_rows;

	auto triangle{generate_triangle(num_rows)};

    show_vectors( std::cout, triangle );

	return 0;
}