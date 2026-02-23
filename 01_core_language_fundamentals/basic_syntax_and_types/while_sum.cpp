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

// Print all books in the library
void printLibrary(const std::vector<Book>& lib) {
    std::cout << "\nLibrary contents:\n";
    for (const auto& b : lib) {
        std::cout << "- " << b.name << " (" << b.publisher << ")\n";
    }
}

// Count total books
int totalBooks(const std::vector<Book>& lib) {
    return static_cast<int>(lib.size());
}

// Check if a book exists by name
bool containsBook(const std::vector<Book>& lib, const std::string& name) {
    for (const auto& b : lib)
        if (b.name == name)
            return true;
    return false;
}

// Find book by ISBN
bool findBookByISBN(const std::vector<Book>& lib, int isbn) {
    for (const auto& b : lib)
        if (b.ISBN_NO == isbn)
            return true;
    return false;
}

void printDivider() {
    std::cout << "-----------------------------\n";
}

struct LibraryStats {
    int visits = 1;
};

std::string toUpperCase(std::string text) {
    for (char& c : text) {
        if (c >= 'a' && c <= 'z')
            c = static_cast<char>(c - ('a' - 'A'));
    }
    return text;
}

// ---------------- SMALL ADDITIONS ----------------

// Add a book to library
void addBook(std::vector<Book>& lib, const Book& b) {
    lib.push_back(b);
}

// Get publisher by book name
std::string getPublisher(const std::vector<Book>& lib, const std::string& name) {
    for (const auto& b : lib)
        if (b.name == name)
            return b.publisher;
    return "Unknown";
}

// Print simple library summary
void printSummary(const std::vector<Book>& lib) {
    std::cout << "Library has " << lib.size() << " books.\n";
}
// ------------------------------------------------


// ======================================================
// MAIN
// ======================================================

int main(void) {

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

    printBook(b1);
    printBook(b2);

    Book sample = createSampleBook();
    printBook(sample);

    std::cout << (hasSameISBN(b2, sample) ? "Same ISBN\n" : "Different ISBN\n");

    std::vector<Book> library = { b1, b2, sample };

    std::cout << "\nTotal books: " << totalBooks(library) << "\n";
    printLibrary(library);

    std::cout << "Contains 'Clean Code'? "
              << (containsBook(library, "Clean Code") ? "Yes\n" : "No\n");

    printDivider();

    std::cout << "Searching ISBN 999: "
              << (findBookByISBN(library, 999) ? "Found\n" : "Not Found\n");

    LibraryStats stats;
    std::cout << "Library visits recorded: " << stats.visits << std::endl;

    std::cout << "Uppercase sample book name: "
              << toUpperCase(sample.name) << std::endl;

    // -------- Added usage --------
    Book extra = { "Design Patterns", 555, "Addison-Wesley" };
    addBook(library, extra);

    printSummary(library);
    std::cout << "Publisher of C++ Primer: "
              << getPublisher(library, "C++ Primer") << std::endl;
    // ----------------------------

    return 0;
}
