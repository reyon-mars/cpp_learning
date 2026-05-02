#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include <numeric>   // added

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

    std::cout << "String created using constructor: "
              << str_v << std::endl;

    // ===== VERY SMALL NEW ADDITIONS =====

    // Print ASCII of first element
    std::cout << "ASCII of first char: "
              << static_cast<int>(v.front()) << std::endl;

    // Check if '!' exists
    std::cout << "Contains '!': "
              << (std::find(v.begin(), v.end(), '!') != v.end()
                  ? "Yes" : "No") << std::endl;

    // Print reversed view (no modification)
    std::cout << "Reversed view: ";
    for (auto it = v.rbegin(); it != v.rend(); ++it)
        std::cout << *it;
    std::cout << std::endl;

    // ===== ADDITIONAL SMALL EXAMPLES =====

    // Count occurrences of 'i'
    std::cout << "Count of 'i': "
              << std::count(v.begin(), v.end(), 'i') << std::endl;

    // Uppercase view (without modifying original vector)
    std::cout << "Uppercase view: ";
    for (char c : v)
        std::cout << static_cast<char>(std::toupper(c));
    std::cout << std::endl;

    // ===================================

    // ===== EXTRA SMALL ADDITIONS (NEW) =====

    // Sort characters (copy, original unchanged)
    std::vector<char> sorted_v = v;
    std::sort(sorted_v.begin(), sorted_v.end());
    std::cout << "Sorted copy: ";
    for (char c : sorted_v)
        std::cout << c;
    std::cout << std::endl;

    // Count total ASCII sum
    int ascii_sum = std::accumulate(v.begin(), v.end(), 0,
        [](int sum, char c) { return sum + static_cast<int>(c); });
    std::cout << "ASCII sum: " << ascii_sum << std::endl;

    // Check if all characters are printable
    bool all_printable = std::all_of(v.begin(), v.end(),
        [](char c) { return std::isprint(static_cast<unsigned char>(c)); });
    std::cout << "All printable? "
              << (all_printable ? "Yes" : "No") << std::endl;

    // ======================================

    // ===== FINAL TINY ADDITIONS =====

    // Convert vector to lowercase string (copy)
    std::string lower = str;
    std::transform(lower.begin(), lower.end(), lower.begin(),
                   [](char c){ return std::tolower(static_cast<unsigned char>(c)); });
    std::cout << "Lowercase string: " << lower << std::endl;

    // Check if vector is palindrome (simple check)
    bool is_palindrome = std::equal(v.begin(), v.begin() + v.size()/2, v.rbegin());
    std::cout << "Is palindrome? "
              << (is_palindrome ? "Yes" : "No") << std::endl;

    // Append more characters safely
    v.push_back('?');
    std::cout << "After adding '?': ";
    for (char c : v) std::cout << c;
    std::cout << std::endl;

    // ======================================

    return 0;
}
