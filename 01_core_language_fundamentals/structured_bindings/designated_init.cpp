#include <algorithm>
#include <cassert>
#include <compare>
#include <iostream>
#include <iterator>
#include <map>
#include <numeric>
#include <optional>
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

	[[nodiscard]] auto operator<=>(const Book&) const = default;
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

[[nodiscard]] int median_isbn(std::vector<Book> lib)
{
	assert(!lib.empty());
	auto mid = lib.begin() + static_cast<std::ptrdiff_t>(lib.size() / 2);
	std::ranges::nth_element(lib, mid, {}, &Book::isbn);
	return mid->isbn;
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

[[nodiscard]] std::optional<Book> find_by_name_copy(std::span<const Book> lib, std::string_view name)
{
	if (const auto* b = find_by_name(lib, name))
		return *b;
	return std::nullopt;
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

[[nodiscard]] bool update_isbn(std::vector<Book>& lib, std::string_view name, int new_isbn)
{
	auto it = std::ranges::find_if(lib, [name](const Book& b) { return b.name == name; });
	if (it == lib.end())
		return false;
	it->isbn = new_isbn;
	return true;
}

void sort_by_name(std::vector<Book>& lib)
{
	std::ranges::sort(lib, {}, &Book::name);
}

void sort_by_isbn(std::vector<Book>& lib)
{
	std::ranges::sort(lib, {}, &Book::isbn);
}

void sort_by_publisher_then_name(std::vector<Book>& lib)
{
	std::ranges::sort(lib,
					  [](const Book& a, const Book& b)
					  {
						  return std::tie(a.publisher, a.name) < std::tie(b.publisher, b.name);
					  });
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

[[nodiscard]] std::vector<Book> books_by_publisher(std::span<const Book> lib, std::string_view publisher)
{
	std::vector<Book> matches;
	std::ranges::copy_if(lib, std::back_inserter(matches),
						 [publisher](const Book& b) { return b.publisher == publisher; });
	return matches;
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

[[nodiscard]] std::map<std::string, std::vector<std::string>> group_names_by_publisher(std::span<const Book> lib)
{
	std::map<std::string, std::vector<std::string>> grouped;
	for (const auto& b : lib)
		grouped[b.publisher].push_back(b.name);
	return grouped;
}

class Library
{
public:
	void add(std::string name, int isbn, std::string publisher)
	{
		books_.push_back({std::move(name), isbn, std::move(publisher)});
	}

	bool remove(std::string_view name)
	{
		return std::erase_if(books_, [name](const Book& b) { return b.name == name; }) > 0;
	}

	[[nodiscard]] std::size_t size()  const noexcept { return books_.size(); }
	[[nodiscard]] bool        empty() const noexcept { return books_.empty(); }

	[[nodiscard]] auto begin()       noexcept { return books_.begin(); }
	[[nodiscard]] auto begin() const noexcept { return books_.begin(); }
	[[nodiscard]] auto end()         noexcept { return books_.end(); }
	[[nodiscard]] auto end()   const noexcept { return books_.end(); }

	[[nodiscard]] operator std::span<const Book>() const noexcept { return books_; }

private:
	std::vector<Book> books_;
};

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
	std::cout << "Median ISBN:  " << median_isbn(library) << '\n';

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

	std::cout << "\nUpdating ISBN of 'Effective C++' to 888:\n";
	if (update_isbn(library, "Effective C++", 888))
		print_book(*find_by_name(library, "Effective C++"));

	std::cout << "\nSort by publisher then name:\n";
	sort_by_publisher_then_name(library);
	print_library(library);

	std::cout << "\nGrouped names by publisher:\n";
	for (const auto& [publisher, names] : group_names_by_publisher(library))
	{
		std::cout << publisher << ": ";
		for (const auto& name : names) std::cout << name << " | ";
		std::cout << '\n';
	}

	std::cout << "\nBooks published by 'Pearson':\n";
	for (const auto& b : books_by_publisher(library, "Pearson"))
		print_book(b);

	std::cout << "\nSafe copy-lookup via optional ('NoSuchBook'):\n";
	if (auto maybe = find_by_name_copy(library, "NoSuchBook"))
		print_book(*maybe);
	else
		std::cout << "Not found, no dangling pointer risk\n";

	std::cout << "\nSame Book value compares equal:\n";
	{
		const Book a{"Same", 42, "Nowhere Press"};
		const Book b{"Same", 42, "Nowhere Press"};
		std::cout << "a == b: " << std::boolalpha << (a == b) << '\n';
	}

	std::cout << "\nLibrary class wrapper:\n";
	Library modern_lib;
	modern_lib.add("The Pragmatic Programmer", 321, "Addison-Wesley");
	modern_lib.add("Refactoring", 654, "Addison-Wesley");
	for (const auto& b : modern_lib) print_book(b);
	std::cout << "size=" << modern_lib.size() << '\n';
	std::cout << "average isbn (via span conversion)=" << average_isbn(modern_lib) << '\n';

	assert(!has_duplicate_isbn(library));
	assert(find_by_name(library, "C++ Primer") != nullptr);
	assert(find_by_name(library, "NoSuchBook") == nullptr);

	std::cout << "\nAll assertions passed.\n";
	std::cout << "Program completed successfully.\n";
	return 0;
}
