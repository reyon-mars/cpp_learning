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

// Print book details using a helper function
void printBook(const Book& b) {
    std::cout << "Name: " << b.name << "\n";
    std::cout << "ISBN: " << b.ISBN_NO << "\n";
    std::cout << "Publisher: " << b.publisher << "\n";
}

// Create a sample book
Book createSampleBook() {
    return {
        "Clean Code",
        10101,
        "Prentice Hall"
    };
}

// Compare ISBN numbers
bool hasSameISBN(const Book& a, const Book& b) {
    return a.ISBN_NO == b.ISBN_NO;
}

// Very small added helper
void printLibrary(const std::vector<Book>& lib) {
    std::cout << "\nLibrary contents:\n";
    for (const auto& b : lib) {
        std::cout << "- " << b.name << " (" << b.publisher << ")\n";
    }
}

// ======================================================
// MAIN
// ======================================================

int main(void) {

    // ---------- ORIGINAL LOGIC ----------
    Book b1 = {
        "test",
        12,
        "O'Rilley"
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

    // ---------- SMALL ADDITION ----------
    std::cout << "\nPrinted using helper function:\n";
    printBook(b1);
    printBook(b2);

    Book sample = createSampleBook();
    std::cout << "\nSample Book:\n";
    printBook(sample);

    std::cout << "\nISBN Comparison Result: ";
    std::cout << (hasSameISBN(b2, sample) ? "Same ISBN\n" : "Different ISBN\n");

    // ---------- VERY SMALL EXTRA ADDITION ----------
    std::vector<Book> library;
    library.push_back(b1);
    library.push_back(b2);
    library.push_back(sample);

    std::cout << "\nTotal books in library: "
              << library.size() << std::endl;

    printLibrary(library);

    return 0;
}
