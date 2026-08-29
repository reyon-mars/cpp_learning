#include <print>
#include <vector>

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

int main()
{
	generate_triangle(4);

	return 0;
}