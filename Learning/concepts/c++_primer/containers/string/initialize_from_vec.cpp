#include <iostream>
#include <vector>
#include <string>

// ==================================================
// ORIGINAL CODE (with SMALL ADDITIONS)
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

    return 0;
}
