// ----------- NEWLY ADDED FUNCTIONS (MORE) -----------

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


// ================= ADD IN MAIN =================

// (Add these lines near the end of main, before return)

// ✅ ADDED: Get full book safely
const Book* found = getBookByName(library, "C++ Primer");
if (found) {
    std::cout << "\nFound book details:\n";
    printBook(*found);
} else {
    std::cout << "Book not found.\n";
}

// ✅ ADDED: Sort books
std::cout << "\nSorting books by name...\n";
sortBooksByName(library);
printLibrary(library);

// ✅ ADDED: Latest book by ISBN
Book latest = getLatestBook(library);
std::cout << "\nLatest book (highest ISBN):\n";
printBook(latest);
