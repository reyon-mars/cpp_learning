#include <iostream>
#include <string>
#include <vector>

struct Book {
    std::string name;
    int ISBN_NO;
    std::string publisher;
};

// Print book details
void printBook(const Book& b) {
    std::cout << "Name: " << b.name << "\n";
    std::cout << "ISBN: " << b.ISBN_NO << "\n";
    std::cout << "Publisher: " << b.publisher << "\n";
}

// Create a sample book
Book createSampleBook() {
    return { "Clean Code", 10101, "Prentice Hall" };
}

// Compare ISBN numbers
bool hasSameISBN(const Book& a, const Book& b) {
    return a.ISBN_NO == b.ISBN_NO;
}

// Print entire library
void printLibrary(const std::vector<Book>& lib) {
    std::cout << "\nLibrary contents:\n";
    for (const auto& b : lib) {
        std::cout << "- " << b.name << " (" << b.publisher << ")\n";
    }
}

// Count books by publisher
int countByPublisher(const std::vector<Book>& lib,
                     const std::string& publisher) {
    int count = 0;
    for (const auto& b : lib)
        if (b.publisher == publisher)
            ++count;
    return count;
}

// Check if book exists by name
bool containsBook(const std::vector<Book>& lib,
                  const std::string& name) {
    for (const auto& b : lib)
        if (b.name == name)
            return true;
    return false;
}

// Print first book safely
void printFirstBook(const std::vector<Book>& lib) {
    if (!lib.empty()) {
        std::cout << "\nFirst book in library:\n";
        printBook(lib.front());
    }
}

// Stream output operator
std::ostream& operator<<(std::ostream& os, const Book& b) {
    return os << b.name << " | ISBN: " << b.ISBN_NO
              << " | Publisher: " << b.publisher;
}

// Find book by ISBN
const Book* findByISBN(const std::vector<Book>& lib, int isbn) {
    for (const auto& b : lib)
        if (b.ISBN_NO == isbn)
            return &b;
    return nullptr;
}

// Remove book by name
bool removeBook(std::vector<Book>& lib, const std::string& name) {
    for (auto it = lib.begin(); it != lib.end(); ++it) {
        if (it->name == name) {
            lib.erase(it);
            return true;
        }
    }
    return false;
}

// ======================================================
// MAIN
// ======================================================

int main() {

    Book b1 = { "test", 12, "O'Reilly" };

    std::cout << "Book Details:\n";
    printBook(b1);

    Book b2;
    b2.name = "C++ Primer";
    b2.ISBN_NO = 999;
    b2.publisher = "Pearson";

    std::cout << "\nSecond Book:\n";
    printBook(b2);

    Book sample = createSampleBook();
    std::cout << "\nSample Book:\n";
    printBook(sample);

    std::cout << "\nISBN Comparison Result: "
              << (hasSameISBN(b2, sample) ? "Same ISBN\n"
                                          : "Different ISBN\n");

    std::vector<Book> library = { b1, b2, sample };

    std::cout << "\nTotal books in library: "
              << library.size() << "\n";

    printLibrary(library);
    printFirstBook(library);

    std::cout << "\nBooks published by Pearson: "
              << countByPublisher(library, "Pearson") << "\n";

    std::cout << "Contains 'Clean Code'? "
              << (containsBook(library, "Clean Code") ? "Yes\n" : "No\n");

    std::cout << "\nPrinted via operator<<:\n";
    std::cout << sample << "\n";

    if (const Book* found = findByISBN(library, 999)) {
        std::cout << "\nFound by ISBN:\n" << *found << "\n";
    }

    removeBook(library, "test");
    std::cout << "\nAfter removing 'test': "
              << library.size() << " books\n";

    return 0;
}
