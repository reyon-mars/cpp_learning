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

    // ===================================

    return 0;
}
