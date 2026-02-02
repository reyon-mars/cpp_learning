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
// SMALL EXTRA FUNCTIONS (ADDED ONLY)
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

// Print all books in the library
void printLibrary(const std::vector<Book>& lib) {
    std::cout << "\nLibrary contents:\n";
    for (const auto& b : lib) {
        std::cout << "- " << b.name << " (" << b.publisher << ")\n";
    }
}

// ---- SMALL ADDED HELPERS ----

// Count total books
int totalBooks(const std::vector<Book>& lib) {
    return lib.size();
}

// Check if a book exists by name
bool containsBook(const std::vector<Book>& lib, const std::string& name) {
    for (const auto& b : lib)
        if (b.name == name)
            return true;
    return false;
}

// ======================================================
// NEW VERY SMALL EXTRA ADDITIONS
// ======================================================

// Find book by ISBN
bool findBookByISBN(const std::vector<Book>& lib, int isbn) {
    for (const auto& b : lib)
        if (b.ISBN_NO == isbn)
            return true;
    return false;
}

// Print divider line
void printDivider() {
    std::cout << "-----------------------------\n";
}

// Simple stats struct
struct LibraryStats {
    int visits = 1;
};

// Convert book name to uppercase (simple demo)
std::string toUpperCase(std::string text) {
    for (char& c : text) {
        if (c >= 'a' && c <= 'z')
            c = c - 32;
    }
    return text;
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

    // ---------- SMALL ADDITION ----------
    printBook(b1);
    printBook(b2);

    Book sample = createSampleBook();
    printBook(sample);

    std::cout << (hasSameISBN(b2, sample) ? "Same ISBN\n" : "Different ISBN\n");

    // ---------- VERY SMALL EXTRA ----------
    std::vector<Book> library = { b1, b2, sample };

    std::cout << "\nTotal books: " << totalBooks(library) << "\n";
    printLibrary(library);

    std::cout << "Contains 'Clean Code'? "
              << (containsBook(library, "Clean Code") ? "Yes\n" : "No\n");

    // ---------- NEW VERY SMALL USAGE ----------
    printDivider();

    std::cout << "Searching ISBN 999: "
              << (findBookByISBN(library, 999) ? "Found\n" : "Not Found\n");

    LibraryStats stats;
    std::cout << "Library visits recorded: " << stats.visits << std::endl;

    std::cout << "Uppercase sample book name: "
              << toUpperCase(sample.name) << std::endl;

    return 0;
}
