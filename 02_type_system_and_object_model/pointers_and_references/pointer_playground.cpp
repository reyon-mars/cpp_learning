#include <algorithm>
#include <cstddef>
#include <iostream>
#include <optional>

template <typename T>
std::optional<T> find_max(T* arr, size_t N)
{
	if (!arr || (N == 0))
	{
		return std::nullopt;
	}

	T max = arr[0];
	for (size_t i = 0; i++ < N;)
	{
		max = std::max(arr[i], max);
	}
	return max;
}

template <typename T>
std::optional<T> find_min(T* arr, size_t N)
{
	if (!arr || (N == 0))
	{
		return std::nullopt;
	}
	T min = arr[0];
	for (size_t i = 0; i++ < N;)
	{
		min = std::min(arr[i], min);
	}
	return min;
}

int second_largest(int* arr, int n)
{
	if (n < 2)
		return -1;

	int largest = arr[0];
	int second = arr[0];

	for (int i = 1; i < n; i++)
	{
		if (arr[i] > largest)
		{
			second = largest;
			largest = arr[i];
		}
		else if (arr[i] > second && arr[i] != largest)
		{
			second = arr[i];
		}
	}

	return second;
}

// Count zeros
int count_zeros(int* arr, int n)
{
	int count = 0;

	for (int i = 0; i < n; i++)
	{
		if (arr[i] == 0)
			count++;
	}

	return count;
}

// Rotate array right by one position
void rotate_right(int* arr, int n)
{
	if (n <= 1)
		return;

	int last = arr[n - 1];

	for (int i = n - 1; i > 0; i--)
	{
		arr[i] = arr[i - 1];
	}

	arr[0] = last;
}

void print_statistics(int* arr, int n)
{
	std::cout << "\nArray Statistics\n";
	std::cout << "Max: " << *find_max(arr, n) << "\n";
	std::cout << "Min: " << *find_min(arr, n) << "\n";
}
