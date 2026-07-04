#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

struct Book
{
	std::string name;
	int ISBN_NO;
	std::string publisher;
};

void printBook(const Book& b)
{
	std::cout << "Name: " << b.name << "\n";
	std::cout << "ISBN: " << b.ISBN_NO << "\n";
	std::cout << "Publisher: " << b.publisher << "\n";
}

void printLibrary(const std::vector<Book>& lib)
{
	for (const auto& b : lib)
	{
		printBook(b);
		std::cout << "------------------\n";
	}
}

std::vector<Book> createSampleLibrary()
{
	return {{"C++ Primer", 123, "Pearson"},
			{"Clean Code", 456, "Prentice Hall"},
			{"Design Patterns", 789, "Addison-Wesley"}};
}

// Find and return a book (pointer version for safety)
const Book* getBookByName(const std::vector<Book>& lib, const std::string& name)
{
	for (const auto& b : lib)
	{
		if (b.name == name)
		{
			return &b;
		}
	}
	return nullptr;
}

// Sort books by name (simple bubble sort for learning)
void sortBooksByName(std::vector<Book>& lib)
{
	for (size_t i = 0; i < lib.size(); ++i)
	{
		for (size_t j = i + 1; j < lib.size(); ++j)
		{
			if (lib[i].name > lib[j].name)
			{
				std::swap(lib[i], lib[j]);
			}
		}
	}
}

// Find book with highest ISBN
Book getLatestBook(const std::vector<Book>& lib)
{
	Book latest = lib[0];

	for (const auto& b : lib)
	{
		if (b.ISBN_NO > latest.ISBN_NO)
		{
			latest = b;
		}
	}

	return latest;
}

void addBook(std::vector<Book>& lib, const std::string& name, int isbn, const std::string& publisher)
{

	lib.push_back({name, isbn, publisher});
}

bool removeBookByISBN(std::vector<Book>& lib, int isbn)
{

	for (auto it = lib.begin(); it != lib.end(); ++it)
	{

		if (it->ISBN_NO == isbn)
		{
			lib.erase(it);
			return true;
		}
	}

	return false;
}

void printBookCount(const std::vector<Book>& lib)
{
	std::cout << "Total books: " << lib.size() << "\n";
}

bool isLibraryEmpty(const std::vector<Book>& lib)
{
	return lib.empty();
}

const Book* getBookByISBN(const std::vector<Book>& lib, int isbn)
{
	for (const auto& b : lib)
	{
		if (b.ISBN_NO == isbn)
		{
			return &b;
		}
	}
	return nullptr;
}

void sortBooksByISBN(std::vector<Book>& lib)
{
	std::sort(lib.begin(),
			  lib.end(),
			  [](const Book& a, const Book& b)
			  {
				  return a.ISBN_NO < b.ISBN_NO;
			  });
}

void printBooksByPublisher(const std::vector<Book>& lib, const std::string& publisher)
{

	std::cout << "\nBooks from publisher: " << publisher << "\n";

	bool found = false;

	for (const auto& b : lib)
	{
		if (b.publisher == publisher)
		{
			printBook(b);
			std::cout << "------------------\n";
			found = true;
		}
	}

	if (!found)
	{
		std::cout << "No books found.\n";
	}
}

double averageISBN(const std::vector<Book>& lib)
{

	if (lib.empty())
		return 0.0;

	long long total = 0;

	for (const auto& b : lib)
	{
		total += b.ISBN_NO;
	}

	return static_cast<double>(total) / lib.size();
}

void libraryStatistics(const std::vector<Book>& lib)
{

	std::cout << "\n=== Library Statistics ===\n";
	std::cout << "Books stored: " << lib.size() << "\n";

	std::cout << "Average ISBN: " << averageISBN(lib) << "\n";
}

int main()
{

	std::vector<Book> library = createSampleLibrary();

	std::cout << "Initial Library:\n";
	printLibrary(library);

	const Book* found = getBookByName(library, "C++ Primer");

	if (found)
	{

		std::cout << "\nFound book details:\n";
		printBook(*found);
	}
	else
	{

		std::cout << "Book not found.\n";
	}

	std::cout << "\nSorting books by name...\n";

	sortBooksByName(library);

	printLibrary(library);

	Book latest = getLatestBook(library);

	std::cout << "\nLatest book (highest ISBN):\n";
	printBook(latest);

	// --------------------------------------------------
	// Add books

	std::cout << "\nAdding new books...\n";

	addBook(library, "Effective C++", 999, "O'Reilly");
	addBook(library, "The Pragmatic Programmer", 888, "Addison-Wesley");

	printLibrary(library);

	printBookCount(library);

	std::cout << "\nRemoving ISBN 456...\n";

	if (removeBookByISBN(library, 456))
	{
		std::cout << "Book removed successfully.\n";
	}
	else
	{
		std::cout << "Book not found.\n";
	}

	printLibrary(library);

	std::cout << "\nLibrary empty? " << (isLibraryEmpty(library) ? "Yes" : "No") << "\n";

	const Book* isbnBook = getBookByISBN(library, 999);

	if (isbnBook)
	{
		std::cout << "\nBook found by ISBN:\n";
		printBook(*isbnBook);
	}

	std::cout << "\nSorting books by ISBN...\n";
	sortBooksByISBN(library);
	printLibrary(library);

	printBooksByPublisher(library, "Addison-Wesley");

	libraryStatistics(library);

	return 0;
}
