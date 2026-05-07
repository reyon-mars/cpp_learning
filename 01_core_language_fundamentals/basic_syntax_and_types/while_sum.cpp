#include <iostream>
#include <string>
#include <vector>
#include <algorithm>   // ✅ FIXED

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

// Print entire library
void printLibrary(const std::vector<Book>& lib) {
    for (const auto& b : lib) {
        printBook(b);
        std::cout << "------------------\n";
    }
}

// ---------------- ORIGINAL SAMPLE DATA ----------------
std::vector<Book> createSampleLibrary() {
    return {
        {"C++ Primer", 123, "Pearson"},
        {"Clean Code", 456, "Prentice Hall"},
        {"Design Patterns", 789, "Addison-Wesley"}
    };
}
// ------------------------------------------------------

// Find and return a book (pointer version for safety)
const Book* getBookByName(const std::vector<Book>& lib, const std::string& name) {
    for (const auto& b : lib) {
        if (b.name == name) {
            return &b;
        }
    }
    return nullptr;
}

// Sort books by name (simple bubble sort for learning)
void sortBooksByName(std::vector<Book>& lib) {
    for (size_t i = 0; i < lib.size(); ++i) {
        for (size_t j = i + 1; j < lib.size(); ++j) {
            if (lib[i].name > lib[j].name) {
                std::swap(lib[i], lib[j]);
            }
        }
    }
}

// Find book with highest ISBN
Book getLatestBook(const std::vector<Book>& lib) {
    Book latest = lib[0];

    for (const auto& b : lib) {
        if (b.ISBN_NO > latest.ISBN_NO) {
            latest = b;
        }
    }

    return latest;
}

// --------------------------------------------------
// ✅ ADDED: Add new book

void addBook(std::vector<Book>& lib,
             const std::string& name,
             int isbn,
             const std::string& publisher) {

    lib.push_back({name, isbn, publisher});
}

// ✅ ADDED: Remove book by ISBN

bool removeBookByISBN(std::vector<Book>& lib, int isbn) {

    for (auto it = lib.begin(); it != lib.end(); ++it) {

        if (it->ISBN_NO == isbn) {
            lib.erase(it);
            return true;
        }
    }

    return false;
}

// ✅ ADDED: Count books

void printBookCount(const std::vector<Book>& lib) {
    std::cout << "Total books: "
              << lib.size() << "\n";
}

// ✅ ADDED: Check if library empty

bool isLibraryEmpty(const std::vector<Book>& lib) {
    return lib.empty();
}

// --------------------------------------------------

// ================= MAIN =================

int main() {

    std::vector<Book> library = createSampleLibrary();

    std::cout << "Initial Library:\n";
    printLibrary(library);

    // ================= ADD IN MAIN =================

    const Book* found = getBookByName(library, "C++ Primer");

    if (found) {

        std::cout << "\nFound book details:\n";
        printBook(*found);

    } else {

        std::cout << "Book not found.\n";
    }

    std::cout << "\nSorting books by name...\n";

    sortBooksByName(library);

    printLibrary(library);

    Book latest = getLatestBook(library);

    std::cout << "\nLatest book (highest ISBN):\n";
    printBook(latest);

    // --------------------------------------------------
    // ✅ ADDED: Add new books

    std::cout << "\nAdding new books...\n";

    addBook(library, "Effective C++", 999, "O'Reilly");
    addBook(library, "The Pragmatic Programmer", 888, "Addison-Wesley");

    printLibrary(library);

    // --------------------------------------------------
    // ✅ ADDED: Book count

    printBookCount(library);

    // --------------------------------------------------
    // ✅ ADDED: Remove a book

    std::cout << "\nRemoving ISBN 456...\n";

    if (removeBookByISBN(library, 456)) {
        std::cout << "Book removed successfully.\n";
    } else {
        std::cout << "Book not found.\n";
    }

    printLibrary(library);

    // --------------------------------------------------
    // ✅ ADDED: Empty check

    std::cout << "\nLibrary empty? "
              << (isLibraryEmpty(library) ? "Yes" : "No")
              << "\n";

    // --------------------------------------------------

    return 0;
}
