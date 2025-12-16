#include  <iostream>
#include <string>
#include <vector>

struct Book{
    std::string name;
    int ISBN_NO;
    std::string publisher;
};

int main ( void ){
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

    return 0;
}

// --------------------------------------------
// EXTRA CODE ADDED BELOW (original unchanged)
// --------------------------------------------

void printBook(const Book& b) {
    std::cout << "\n[Extra] Book Info:\n";
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

int _ = [](){
    Book sample = createSampleBook();
    printBook(sample);
    return 0;
}();

// --------------------------------------------
// MORE EXTRA CODE ADDED BELOW (still unchanged above)
// --------------------------------------------

bool hasSameISBN(const Book& a, const Book& b) {
    return a.ISBN_NO == b.ISBN_NO;
}

void printBookList(const std::vector<Book>& books) {
    std::cout << "\n[Extra] Book List:\n";
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

int __extra_init = [](){
    std::cout << "\n--- Extra Code Execution ---\n";

    Book a = {"Dummy Book", 999, "Unknown"};
    Book b = {"Another Dummy", 999, "Unknown"};

    std::cout << "Checking ISBN equality: "
              << (hasSameISBN(a, b) ? "Same" : "Different") << "\n";

    auto collection = generateBookCollection();
    printBookList(collection);

    return 0;
}();

// --------------------------------------------
// FINAL SMALL EXTRA ADDITION (appended only)
// --------------------------------------------

const Book* findByISBN(const std::vector<Book>& books, int isbn) {
    for (const auto& b : books) {
        if (b.ISBN_NO == isbn)
            return &b;
    }
    return nullptr;
}

int ___ = [](){
    auto books = generateBookCollection();
    const Book* found = findByISBN(books, 11111);

    if (found) {
        std::cout << "\n[Extra] Found book by ISBN:\n";
        printBook(*found);
    }
    return 0;
}();
