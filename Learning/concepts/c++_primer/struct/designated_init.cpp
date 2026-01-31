#include <iostream>
#include <string>
#include <vector>

// ======================================================
// ORIGINAL CODE (LOGIC UNCHANGED)
// ======================================================

struct Book {
    std::string name;
    int ISBN_NO;
    std::string publisher;
};

// ======================================================
// SMALL EXTRA FUNCTIONS (ADDITION ONLY)
// ======================================================

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

int main(void) {

    // ---------- ORIGINAL LOGIC ----------
    Book b1 = { "test", 12, "O'Rilley" };

    std::cout << "Book Details:\n";
    std::cout << "Name: " << b1.name << std::endl;
    std::cout << "ISBN: " << b1.ISBN_NO << std::endl;
    std::cout << "Publisher: " << b1.publisher << std::endl;

    Book b2;
    b2.name = "C++ Primer";
    b2.ISBN_NO = 999;
    b2.publisher = "Pearson";

    std::cout << "\nSecond Book:\n";
    std::cout << "Name: " << b2.name << std::endl;
    std::cout << "ISBN: " << b2.ISBN_NO << std::endl;
    std::cout << "Publisher: " << b2.publisher << std::endl;

    // ---------- SMALL ADDITIONS ----------
    std::cout << "\nPrinted using helper function:\n";
    printBook(b1);
    printBook(b2);

    Book sample = createSampleBook();
    std::cout << "\nSample Book:\n";
    printBook(sample);

    std::cout << "\nISBN Comparison Result: "
              << (hasSameISBN(b2, sample) ? "Same ISBN\n" : "Different ISBN\n");

    // ---------- VERY SMALL EXTRA ADDITIONS ----------
    std::vector<Book> library = { b1, b2, sample };

    std::cout << "\nTotal books in library: "
              << library.size() << std::endl;

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
