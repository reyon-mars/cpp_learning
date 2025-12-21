#include <iostream>
#include <string>
#include <vector>

// ======================================================
// ORIGINAL CODE (LOGIC UNCHANGED)
// ======================================================

struct Book{
    std::string name;
    int ISBN_NO;
    std::string publisher;
};

// ======================================================
// EXTRA FUNCTIONS
// ======================================================

void printBook(const Book& b) {
    std::cout << "\nBook Info:\n";
    std::cout << "Name: " << b.name << "\n";
    std::cout << "ISBN: " << b.ISBN_NO << "\n";
    std::cout << "Publisher: " << b.publisher << "\n";
}

Book createSampleBook() {
    return Book{
        .name = "Clean Code",
        .ISBN_NO = 10101,
        .publisher = "Prentice Hall"
    };
}

bool hasSameISBN(const Book& a, const Book& b) {
    return a.ISBN_NO == b.ISBN_NO;
}

void printBookList(const std::vector<Book>& books) {
    std::cout << "\nBook List:\n";
    for (const auto& book : books) {
        std::cout << "- " << book.name
                  << " (ISBN: " << book.ISBN_NO
                  << ", Publisher: " << book.publisher
                  << ")\n";
    }
}

std::vector<Book> generateBookCollection() {
    return {
        {"Design Patterns", 12345, "Addison-Wesley"},
        {"The Pragmatic Programmer", 54321, "Addison-Wesley"},
        {"Effective Modern C++", 11111, "O'Reilly"},
    };
}

const Book* findByISBN(const std::vector<Book>& books, int isbn) {
    for (const auto& b : books) {
        if (b.ISBN_NO == isbn)
            return &b;
    }
    return nullptr;
}

// ======================================================
// MAIN
// ======================================================

int main ( void ){

    // ---------- ORIGINAL LOGIC ----------
    Book b1 = {
        .name = "test",
        .ISBN_NO = 12,
        .publisher = "O'Rilley"
    };

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

    // ---------- EXTRA DEMONSTRATIONS ----------

    // Print using helper
    printBook(b1);
    printBook(b2);

    // Factory function
    Book sample = createSampleBook();
    printBook(sample);

    // ISBN comparison
    std::cout << "\nISBN Comparison:\n";
    std::cout << (hasSameISBN(b2, sample) ? "Same ISBN\n" : "Different ISBN\n");

    // Collection of books
    auto collection = generateBookCollection();
    printBookList(collection);

    // Find book by ISBN
    int searchISBN = 11111;
    const Book* found = findByISBN(collection, searchISBN);

    if (found) {
        std::cout << "\nFound book with ISBN " << searchISBN << ":\n";
        printBook(*found);
    } else {
        std::cout << "\nBook with ISBN " << searchISBN << " not found\n";
    }

    return 0;
}
