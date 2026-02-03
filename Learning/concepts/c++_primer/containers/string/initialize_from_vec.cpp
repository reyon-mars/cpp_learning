#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

// ==================================================
// ORIGINAL CODE (LOGIC UNCHANGED)
// ==================================================

int main(void) {
    std::vector<char> v;

    // --- small extra (capacity hint) ---
    v.reserve(10);
    // ----------------------------------

    // --- original additions ---
    v.push_back('H');
    v.push_back('i');
    v.push_back('!');
    // --------------------------

    std::string str_v(v.begin(), v.end());
    std::string str;

    for (auto c : v)
        str += c;

    std::cout << str << std::endl;

    // --- small added code ---
    std::cout << "Vector size: " << v.size() << std::endl;
    std::cout << "Vector capacity: " << v.capacity() << std::endl;

    if (!v.empty()) {
        std::cout << "First character: " << v.front() << std::endl;
        std::cout << "Last character: " << v.back() << std::endl;
    }
    // -----------------------

    // --- tiny extra output ---
    std::cout << "Vector as C-string style: ";
    for (char c : v)
        std::cout << c;
    std::cout << std::endl;
    // ------------------------

    std::cout << "String created using constructor: "
              << str_v << std::endl;

    // --- VERY SMALL EXTRA ADDITION ---
    std::cout << "Is vector empty? "
              << (v.empty() ? "Yes" : "No") << std::endl;

    std::cout << "ASCII values: ";
    for (char c : v)
        std::cout << static_cast<int>(c) << " ";
    std::cout << std::endl;

    // ===== NEW VERY SMALL EXTRAS =====

    // Count uppercase letters
    int uppercase_count = 0;
    for (char c : v)
        if (c >= 'A' && c <= 'Z')
            uppercase_count++;

    std::cout << "Uppercase letters count: "
              << uppercase_count << std::endl;

    // Check if vector contains 'i'
    bool contains_i =
        std::find(v.begin(), v.end(), 'i') != v.end();

    std::cout << "Contains 'i'? "
              << (contains_i ? "Yes" : "No") << std::endl;

    // Print reversed characters (without modifying vector)
    std::cout << "Reversed view: ";
    for (auto it = v.rbegin(); it != v.rend(); ++it)
        std::cout << *it;
    std::cout << std::endl;

    // =================================

    return 0;
}
