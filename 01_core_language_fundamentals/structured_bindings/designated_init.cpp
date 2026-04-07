// ----------- MORE ADVANCED ADDITIONS -----------

// Find using STL algorithm
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

// Binary search (requires sorted vector)
const Book* binarySearchISBN(const std::vector<Book>& lib, int isbn) {
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

// Average ISBN value
double averageISBN(const std::vector<Book>& lib) {
    if (lib.empty()) return 0.0;
    return static_cast<double>(totalISBN(lib)) / lib.size();
}

// ----------------------------------------------


// ================= ADD IN MAIN =================

// (Add near the end before return)

std::cout << "\nAdvanced Features:\n";

// ✅ STL find
if (const Book* b = findByNameSTL(library, "Clean Code")) {
    std::cout << "Found using STL: " << *b << "\n";
}

// ✅ Binary search (after sorting)
if (const Book* b = binarySearchISBN(library, 999)) {
    std::cout << "Found using binary search: " << *b << "\n";
}

// ✅ Average ISBN
std::cout << "Average ISBN: "
          << averageISBN(library) << "\n";

// ✅ Modern remove
removeBookModern(library, "Clean Code");
std::cout << "After modern remove, size: "
          << library.size() << "\n";
