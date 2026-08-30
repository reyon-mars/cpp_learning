#include <algorithm>
#include <cassert>
#include <format>
#include <iostream>
#include <iterator>
#include <numeric>
#include <ostream>
#include <print>
#include <ranges>
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

bool is_palindrome( const std::vector<int>& row )
{
    auto forward = row | std::ranges::views::take( row.size() >> 1 );

    auto backward = row | std::ranges::views::reverse | std::ranges::views::take( row.size() >> 1 );

    return std::ranges::equal(forward, backward );
}

void check_properties(const std::vector<std::vector<int>>& triangle)
{
	size_t row_num{1};
	int expected_sum{1};

	for (const auto& row : triangle)
	{
		assert(row.front() == 1);
		assert(row.back() == 1);

		assert(row.size() == row_num);

        auto negatives = row | std::ranges::views::filter([]( int x ) { return x < 0; });
        
        assert( negatives.empty() );

		assert(expected_sum == std::accumulate(row.begin(), row.end(), 0));
        assert( std::ranges::all_of(row, []( auto x ){ return x >= 0; }));

        assert( is_palindrome( row ) );

		row_num++;
		expected_sum <<= 1;
	}
}

int main()
{
	int num_rows{};
	while (true)
	{
		std::println("Please enter the number of rows: ");
		std::cin >> num_rows;

		auto triangle{generate_triangle(num_rows)};
        check_properties(triangle);

		show_vectors(std::cout, triangle);
	}
	return 0;
}