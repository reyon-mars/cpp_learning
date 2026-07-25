#include <algorithm>
#include <cstddef>
#include <iostream>
#include <numeric>
#include <optional>
#include <span>
#include <string_view>
#include <vector>

template <typename T>
[[nodiscard]] std::optional<T> find_max(const T* arr, std::size_t n)
{
	if (!arr || n == 0)
		return std::nullopt;
	T max = arr[0];
	for (std::size_t i = 1; i < n; ++i)
		max = std::max(arr[i], max);
	return max;
}

template <typename T>
[[nodiscard]] std::optional<T> find_min(const T* arr, std::size_t n)
{
	if (!arr || n == 0)
		return std::nullopt;
	T min = arr[0];
	for (std::size_t i = 1; i < n; ++i)
		min = std::min(arr[i], min);
	return min;
}

template <typename T>
[[nodiscard]] std::optional<T> find_max(std::span<const T> values)
{
	return find_max(values.data(), values.size());
}

template <typename T>
[[nodiscard]] std::optional<T> find_min(std::span<const T> values)
{
	return find_min(values.data(), values.size());
}

[[nodiscard]] std::optional<int> second_largest(std::span<const int> values)
{
	if (values.size() < 2)
		return std::nullopt;
	int largest = values[0];
	std::optional<int> second;
	for (std::size_t i = 1; i < values.size(); ++i)
	{
		if (values[i] > largest)
		{
			second = largest;
			largest = values[i];
		}
		else if (values[i] != largest && (!second || values[i] > *second))
		{
			second = values[i];
		}
	}
	return second;
}

[[nodiscard]] std::size_t count_zeros(std::span<const int> values)
{
	return static_cast<std::size_t>(std::ranges::count(values, 0));
}

void rotate_right(std::span<int> values)
{
	if (values.size() <= 1)
		return;
	std::rotate(values.rbegin(), values.rbegin() + 1, values.rend());
}

void rotate_left(std::span<int> values)
{
	if (values.size() <= 1)
		return;
	std::rotate(values.begin(), values.begin() + 1, values.end());
}

[[nodiscard]] double average(std::span<const int> values)
{
	if (values.empty())
		return 0.0;
	const auto sum = std::accumulate(values.begin(), values.end(), 0LL);
	return static_cast<double>(sum) / static_cast<double>(values.size());
}

void print_values(std::span<const int> values, std::string_view label)
{
	std::cout << label << ": ";
	for (int v : values)
		std::cout << v << ' ';
	std::cout << '\n';
}

void print_statistics(std::span<const int> values)
{
	std::cout << "\nArray Statistics\n";
	if (const auto max = find_max(values))
		std::cout << "Max: " << *max << "\n";
	if (const auto min = find_min(values))
		std::cout << "Min: " << *min << "\n";
	std::cout << "Sum: " << std::accumulate(values.begin(), values.end(), 0) << "\n";
	std::cout << "Average: " << average(values) << "\n";
	if (const auto second = second_largest(values))
		std::cout << "Second largest: " << *second << "\n";
	else
		std::cout << "Second largest: none (fewer than two distinct values)\n";
	std::cout << "Zero count: " << count_zeros(values) << "\n";
}

int main()
{
	std::vector<int> numbers{3, 1, 4, 1, 5, 9, 2, 6, 0, 0};

	print_values(numbers, "Original");
	print_statistics(numbers);

	std::cout << "\nEdge cases\n";
	const std::vector<int> empty;
	std::cout << "find_max on empty has value: " << std::boolalpha << find_max<int>(empty).has_value() << "\n";
	const std::vector<int> single{42};
	std::cout << "find_max on single-element: " << *find_max<int>(single) << "\n";
	std::cout << "second_largest on single-element has value: " << second_largest(single).has_value() << "\n";
	const std::vector<int> all_same{7, 7, 7};
	std::cout << "second_largest on all-equal has value: " << second_largest(all_same).has_value() << "\n";

	std::cout << "\nRotation\n";
	auto rotated = numbers;
	rotate_right(rotated);
	print_values(rotated, "After rotate_right");
	rotate_left(rotated);
	print_values(rotated, "After rotate_left (back to original)");

	std::cout << "\nRaw pointer overload still works\n";
	int raw_array[]{10, 20, 5, 40, 15};
	if (const auto max = find_max(raw_array, std::size(raw_array)))
		std::cout << "raw_array max=" << *max << "\n";

	return 0;
}
