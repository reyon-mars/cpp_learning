#include <iostream>
#include <string>
#include <vector>
#include <algorithm> // ✅ ADDED
#include <numeric>   // ✅ ADDED
#include <cassert>   // ✅ ADDED
#include <map>       // ✅ ADDED
#include <set>       // ✅ ADDED

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

// Sum of ISBNs
int totalISBN(const std::vector<Book>& lib) {
    int sum = 0;
    for (const auto& b : lib) sum += b.ISBN_NO;
    return sum;
}

// ----------- PREVIOUS ADDITIONS -----------

// Find book by name (manual)
const Book* getBookByName(const std::vector<Book>& lib, const std::string& name) {
    for (const auto& b : lib) {
        if (b.name == name) {
            return &b;
        }
    }
    return nullptr;
}

// Sort books by name
void sortBooksByName(std::vector<Book>& lib) {
    for (size_t i = 0; i < lib.size(); ++i) {
        for (size_t j = i + 1; j < lib.size(); ++j) {
            if (lib[i].name > lib[j].name) {
                std::swap(lib[i], lib[j]);
            }
        }
    }
}

// Find latest book
Book getLatestBook(const std::vector<Book>& lib) {
    Book latest = lib[0];
    for (const auto& b : lib) {
        if (b.ISBN_NO > latest.ISBN_NO) {
            latest = b;
        }
    }
    return latest;
}

// ----------- MORE ADVANCED ADDITIONS -----------

// STL find
const Book* findByNameSTL(const std::vector<Book>& lib,
                          const std::string& name) {
    auto it = std::find_if(lib.begin(), lib.end(),
        [&](const Book& b) { return b.name == name; });

    return (it != lib.end()) ? &(*it) : nullptr;
}

// Remove using erase-remove idiom
void removeBookModern(std::vector<Book>& lib,
                      const std::string& name) {
    lib.erase(
        std::remove_if(lib.begin(), lib.end(),
            [&](const Book& b) { return b.name == name; }),
        lib.end()
    );
}

// Binary search (requires sorted vector by ISBN)
const Book* binarySearchISBN(const std::vector<Book>& lib, int isbn) {
    assert(!lib.empty());  // ✅ ADDED safety check

    int left = 0;
    int right = static_cast<int>(lib.size()) - 1;

    while (left <= right) {
        int mid = (left + right) / 2;

        if (lib[mid].ISBN_NO == isbn)
            return &lib[mid];
        else if (lib[mid].ISBN_NO < isbn)
            left = mid + 1;
        else
            right = mid - 1;
    }
    return nullptr;
}

// Average ISBN
double averageISBN(const std::vector<Book>& lib) {
    if (lib.empty()) return 0.0;
    return static_cast<double>(totalISBN(lib)) / lib.size();
}

// Sort by ISBN (needed for binary search)
void sortByISBN(std::vector<Book>& lib) {
    std::sort(lib.begin(), lib.end(),
        [](const Book& a, const Book& b) {
            return a.ISBN_NO < b.ISBN_NO;
        });
}

// ----------- NEW SMALL ADDITIONS -----------

// Count books by publisher
void countPublishers(const std::vector<Book>& lib) {
    std::map<std::string, int> counts;

    for (const auto& b : lib) {
        counts[b.publisher]++;
    }

    std::cout << "\nBooks per publisher:\n";
    for (const auto& [publisher, count] : counts) {
        std::cout << publisher << ": " << count << "\n";
    }
}

// Check duplicate ISBNs
bool hasDuplicateISBN(const std::vector<Book>& lib) {
    std::set<int> seen;

    for (const auto& b : lib) {
        if (!seen.insert(b.ISBN_NO).second) {
            return true;
        }
    }
    return false;
}

// Add a new book safely
void addBook(std::vector<Book>& lib,
             const std::string& name,
             int isbn,
             const std::string& publisher) {

    lib.push_back({name, isbn, publisher});
}

// Find minimum ISBN book
Book getEarliestBook(const std::vector<Book>& lib) {
    Book earliest = lib[0];

    for (const auto& b : lib) {
        if (b.ISBN_NO < earliest.ISBN_NO) {
            earliest = b;
        }
    }

    return earliest;
}

// ------------------------------------------------

int main() {

    std::vector<Book> library = {
        {"C++ Primer", 111, "Pearson"},
        {"Clean Code", 999, "Prentice Hall"},
        {"Design Patterns", 555, "Addison-Wesley"}
    };

    std::cout << "Initial Library:\n";
    printLibrary(library);

    // -------- EXISTING FEATURES --------

    const Book* found = getBookByName(library, "C++ Primer");
    if (found) {
        std::cout << "\nFound book:\n";
        printBook(*found);
    }

    std::cout << "\nSorting by name:\n";
    sortBooksByName(library);
    printLibrary(library);

    Book latest = getLatestBook(library);
    std::cout << "\nLatest book:\n";
    printBook(latest);

    // ================= NEW ADDITIONS =================

    std::cout << "\nAdvanced Features:\n";

    // STL find
    if (const Book* b = findByNameSTL(library, "Clean Code")) {
        std::cout << "Found using STL:\n";
        printBook(*b);
    }

    // Sort by ISBN before binary search
    sortByISBN(library);

    if (const Book* b = binarySearchISBN(library, 999)) {
        std::cout << "Found using binary search:\n";
        printBook(*b);
    }

    // Average ISBN
    std::cout << "Average ISBN: "
              << averageISBN(library) << "\n";

    // Modern remove
    removeBookModern(library, "Clean Code");

    std::cout << "\nAfter removing 'Clean Code':\n";
    printLibrary(library);

    std::cout << "Library size: " << library.size() << "\n";

    // =================================================

    // ================= EXTRA SMALL FEATURES =================

    addBook(library, "Effective C++", 777, "OReilly");

    std::cout << "\nAfter adding new book:\n";
    printLibrary(library);

    Book earliest = getEarliestBook(library);

    std::cout << "\nEarliest book (lowest ISBN):\n";
    printBook(earliest);

    countPublishers(library);

    std::cout << "\nDuplicate ISBN check: "
              << (hasDuplicateISBN(library) ? "Duplicates found"
                                            : "No duplicates")
              << "\n";

    // ========================================================

    return 0;
}
