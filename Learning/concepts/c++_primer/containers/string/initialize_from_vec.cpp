#include <iostream>
#include <vector>
#include <string>

// ==================================================
// ORIGINAL CODE (with SMALL ADDITION)
// ==================================================

int main(void) {
    std::vector<char> v;

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

    if (!v.empty()) {
        std::cout << "Last character: " << v.back() << std::endl;
    }
    // -----------------------

    std::cout << "String created using constructor: "
              << str_v << std::endl;

    return 0;
}
