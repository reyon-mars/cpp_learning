#include <algorithm>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <format>
#include <ranges>
#include <optional>
#include <numeric>
#include <cassert>
#include <span>
#include <concepts>
#include <functional>

struct Book {
    std::string name;
    int         isbn{};
    std::string publisher;

    auto operator<=>(const Book&) const = default;
};

void print_book(const Book& b) {
    std::cout << std::format("Name:      {}\nISBN:      {}\nPublisher: {}\n",
                             b.name, b.isbn, b.publisher);
}

void print_divider() { std::cout << "------------------\n"; }

template <std::ranges::input_range R>
void print_library(const R& lib) {
    for (const auto& b : lib) { print_book(b); print_divider(); }
}

[[nodiscard]] std::vector<Book> create_sample_library() {
    return {
        {"C++ Primer",           123, "Pearson"},
        {"Clean Code",           456, "Prentice Hall"},
        {"Design Patterns",      789, "Addison-Wesley"},
    };
}

[[nodiscard]] std::optional<std::reference_wrapper<const Book>>
find_by_name(const std::vector<Book>& lib, std::string_view name) {
    const auto it = std::ranges::find_if(lib, [&](const Book& b) { return b.name == name; });
    if (it == lib.end()) return std::nullopt;
    return std::cref(*it);
}

[[nodiscard]] std::optional<std::reference_wrapper<const Book>>
find_by_isbn(const std::vector<Book>& lib, int isbn) {
    const auto it = std::ranges::find_if(lib, [&](const Book& b) { return b.isbn == isbn; });
    if (it == lib.end()) return std::nullopt;
    return std::cref(*it);
}

[[nodiscard]] std::optional<Book> latest_book(const std::vector<Book>& lib) {
    if (lib.empty()) return std::nullopt;
    return *std::ranges::max_element(lib, {}, &Book::isbn);
}

void sort_by_name (std::vector<Book>& lib) { std::ranges::sort(lib, {}, &Book::name); }
void sort_by_isbn (std::vector<Book>& lib) { std::ranges::sort(lib, {}, &Book::isbn); }

void add_book(std::vector<Book>& lib, std::string name, int isbn, std::string publisher) {
    lib.push_back({std::move(name), isbn, std::move(publisher)});
}

[[nodiscard]] bool remove_by_isbn(std::vector<Book>& lib, int isbn) {
    const auto [first, last] = std::ranges::remove_if(lib, [&](const Book& b) {
        return b.isbn == isbn;
    });
    if (first == last) return false;
    lib.erase(first, last);
    return true;
}

void print_by_publisher(const std::vector<Book>& lib, std::string_view publisher) {
    std::cout << std::format("\nBooks from publisher: {}\n", publisher);
    auto matching = lib | std::views::filter([&](const Book& b) { return b.publisher == publisher; });
    if (std::ranges::empty(matching)) {
        std::cout << "No books found.\n";
        return;
    }
    print_library(matching);
}

[[nodiscard]] double average_isbn(const std::vector<Book>& lib) {
    if (lib.empty()) return 0.0;
    const long long total = std::reduce(lib.begin(), lib.end(), 0LL,
                                        [](long long acc, const Book& b) { return acc + b.isbn; });
    return static_cast<double>(total) / static_cast<double>(lib.size());
}

void library_statistics(const std::vector<Book>& lib) {
    std::cout << std::format("\n=== Library Statistics ===\n"
                             "Books stored:  {}\n"
                             "Average ISBN:  {:.1f}\n",
                             lib.size(), average_isbn(lib));

    if (const auto latest = latest_book(lib))
        std::cout << std::format("Highest ISBN:  {} ({})\n", latest->isbn, latest->name);

    const auto publishers = lib
        | std::views::transform([](const Book& b) -> std::string_view { return b.publisher; });
    std::cout << std::format("Empty: {}\n", lib.empty() ? "Yes" : "No");

    const auto unique_publishers = lib
        | std::views::transform(&Book::publisher)
        | std::ranges::to<std::vector>()
        | [](auto v) { std::ranges::sort(v); auto [e,_] = std::ranges::unique(v); v.erase(e,v.end()); return v; };
    std::cout << std::format("Publishers: {}\n", unique_publishers.size());
}

int main() {
    std::vector<Book> library = create_sample_library();

    std::cout << "Initial Library:\n";
    print_library(library);

    if (const auto found = find_by_name(library, "C++ Primer")) {
        std::cout << "\nFound book details:\n";
        print_book(found->get());
    } else {
        std::cout << "Book not found.\n";
    }

    std::cout << "\nSorting books by name...\n";
    sort_by_name(library);
    print_library(library);

    if (const auto latest = latest_book(library)) {
        std::cout << "\nLatest book (highest ISBN):\n";
        print_book(*latest);
    }

    std::cout << "\nAdding new books...\n";
    add_book(library, "Effective C++",            999, "O'Reilly");
    add_book(library, "The Pragmatic Programmer", 888, "Addison-Wesley");
    print_library(library);
    std::cout << std::format("Total books: {}\n", library.size());

    std::cout << "\nRemoving ISBN 456...\n";
    std::cout << (remove_by_isbn(library, 456)
                 ? "Book removed successfully.\n"
                 : "Book not found.\n");
    print_library(library);

    std::cout << std::format("\nLibrary empty? {}\n", library.empty() ? "Yes" : "No");

    if (const auto found = find_by_isbn(library, 999)) {
        std::cout << "\nBook found by ISBN:\n";
        print_book(found->get());
    }

    std::cout << "\nSorting books by ISBN...\n";
    sort_by_isbn(library);
    print_library(library);

    print_by_publisher(library, "Addison-Wesley");
    library_statistics(library);

    std::cout << "\n--- views pipeline ---\n";
    auto high_isbn = library
        | std::views::filter([](const Book& b) { return b.isbn > 500; })
        | std::views::transform(&Book::name);
    std::cout << "Books with ISBN > 500:\n";
    for (std::string_view name : high_isbn) std::cout << std::format("  {}\n", name);

    assert(!find_by_isbn(library, 456).has_value());
    assert(find_by_isbn(library, 999).has_value());
    assert(find_by_name(library, "Effective C++").has_value());
    assert(!find_by_name(library, "Unknown Book").has_value());
    assert(latest_book({}).has_value() == false);

    std::cout << "\nAll assertions passed.\n";
    return 0;
}
