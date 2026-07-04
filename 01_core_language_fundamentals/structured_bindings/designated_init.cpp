#include <algorithm>
#include <cassert>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

struct Book
{
	std::string name;
	int isbn = 0;
	std::string publisher;
};

void print_book(const Book& b)
{
	std::cout << "Name:      " << b.name << '\n'
			  << "ISBN:      " << b.isbn << '\n'
			  << "Publisher: " << b.publisher << '\n';
}

void print_library(std::span<const Book> lib)
{
	for (const auto& b : lib)
	{
		print_book(b);
		std::cout << "------------------\n";
	}
}

[[nodiscard]] int total_isbn(std::span<const Book> lib) noexcept
{
	return std::accumulate(lib.begin(),
						   lib.end(),
						   0,
						   [](int sum, const Book& b) noexcept
						   {
							   return sum + b.isbn;
						   });
}

[[nodiscard]] double average_isbn(std::span<const Book> lib) noexcept
{
	if (lib.empty())
		return 0.0;
	return static_cast<double>(total_isbn(lib)) / lib.size();
}

[[nodiscard]] const Book* find_by_name(std::span<const Book> lib, std::string_view name) noexcept
{
	auto it = std::ranges::find_if(lib,
								   [name](const Book& b)
								   {
									   return b.name == name;
								   });
	return it != lib.end() ? &*it : nullptr;
}

[[nodiscard]] const Book* find_by_isbn(std::span<const Book> lib, int isbn) noexcept
{
	assert(!lib.empty());
	int left = 0, right = static_cast<int>(lib.size()) - 1;
	while (left <= right)
	{
		const int mid = left + (right - left) / 2;
		if (lib[mid].isbn == isbn)
			return &lib[mid];
		else if (lib[mid].isbn < isbn)
			left = mid + 1;
		else
			right = mid - 1;
	}
	return nullptr;
}

[[nodiscard]] const Book* latest_book(std::span<const Book> lib) noexcept
{
	return &*std::ranges::max_element(lib, {}, &Book::isbn);
}

[[nodiscard]] const Book* earliest_book(std::span<const Book> lib) noexcept
{
	return &*std::ranges::min_element(lib, {}, &Book::isbn);
}

void add_book(std::vector<Book>& lib, std::string name, int isbn, std::string publisher)
{
	lib.push_back({std::move(name), isbn, std::move(publisher)});
}

void remove_book(std::vector<Book>& lib, std::string_view name)
{
	std::erase_if(lib,
				  [name](const Book& b)
				  {
					  return b.name == name;
				  });
}

void sort_by_name(std::vector<Book>& lib)
{
	std::ranges::sort(lib, {}, &Book::name);
}

void sort_by_isbn(std::vector<Book>& lib)
{
	std::ranges::sort(lib, {}, &Book::isbn);
}

[[nodiscard]] bool has_duplicate_isbn(std::span<const Book> lib)
{
	std::set<int> seen;
	return std::ranges::any_of(lib,
							   [&](const Book& b)
							   {
								   return !seen.insert(b.isbn).second;
							   });
}

void count_publishers(std::span<const Book> lib)
{
	std::map<std::string_view, int> counts;
	for (const auto& b : lib)
		++counts[b.publisher];
	std::cout << "\nBooks per publisher:\n";
	for (const auto& [pub, cnt] : counts)
		std::cout << pub << ": " << cnt << '\n';
}

int main()
{
	std::vector<Book> library = {
		{"C++ Primer", 111, "Pearson"},
		{"Clean Code", 999, "Prentice Hall"},
		{"Design Patterns", 555, "Addison-Wesley"},
	};

	std::cout << "Initial Library:\n";
	print_library(library);

	if (const auto* b = find_by_name(library, "C++ Primer"))
	{
		std::cout << "\nFound by name:\n";
		print_book(*b);
	}

	std::cout << "\nSorted by name:\n";
	sort_by_name(library);
	print_library(library);

	std::cout << "\nLatest (highest ISBN):\n";
	print_book(*latest_book(library));

	std::cout << "\nAdvanced Features:\n";

	if (const auto* b = find_by_name(library, "Clean Code"))
	{
		std::cout << "Found (STL):\n";
		print_book(*b);
	}

	sort_by_isbn(library);

	if (const auto* b = find_by_isbn(library, 999))
	{
		std::cout << "Found (binary search):\n";
		print_book(*b);
	}

	std::cout << "Average ISBN: " << average_isbn(library) << '\n';

	remove_book(library, "Clean Code");
	std::cout << "\nAfter removing 'Clean Code':\n";
	print_library(library);
	std::cout << "Library size: " << library.size() << '\n';

	add_book(library, "Effective C++", 777, "OReilly");
	std::cout << "\nAfter adding 'Effective C++':\n";
	print_library(library);

	std::cout << "\nEarliest (lowest ISBN):\n";
	print_book(*earliest_book(library));

	count_publishers(library);

	std::cout << "\nDuplicate ISBNs? " << (has_duplicate_isbn(library) ? "Yes" : "No") << '\n';

	assert(!has_duplicate_isbn(library));
	assert(find_by_name(library, "C++ Primer") != nullptr);
	assert(find_by_name(library, "NoSuchBook") == nullptr);

	std::cout << "\nAll assertions passed.\n";
	std::cout << "Program completed successfully.\n";
	return 0;
}
