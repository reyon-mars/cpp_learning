#include <iostream>
#include <string>
#include <vector>
#include <algorithm> /

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

    return 0;
}
