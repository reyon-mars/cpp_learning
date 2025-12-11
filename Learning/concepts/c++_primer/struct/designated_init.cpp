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

// Compare books by ISBN
bool hasSameISBN(const Book& a, const Book& b) {
    return a.ISBN_NO == b.ISBN_NO;
}

// Print a list of books
void printBookList(const std::vector<Book>& books) {
    std::cout << "\n[Extra] Book List:\n";
    for (const auto& book : books) {
        std::cout << "- " << book.name 
                  << " (ISBN: " << book.ISBN_NO 
                  << ", Publisher: " << book.publisher 
                  << ")\n";
    }
}

// Return a sample collection of books
std::vector<Book> generateBookCollection() {
    return {
        {"Design Patterns", 12345, "Addison-Wesley"},
        {"The Pragmatic Programmer", 54321, "Addison-Wesley"},
        {"Effective Modern C++", 11111, "O'Reilly"},
    };
}

// Executes after main (like your previous lambda)
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
