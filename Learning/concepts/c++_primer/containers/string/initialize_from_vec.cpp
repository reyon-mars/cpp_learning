#include <iostream>
#include <vector>
#include <string>

// ==================================================
// ORIGINAL CODE (UNCHANGED)
// ==================================================

int main(void) {
    std::vector<char> v;

    // --- added lines ---
    v.push_back('H');
    v.push_back('i');
    v.push_back('!');
    // --------------------

    std::string str_v(v.begin(), v.end());
    std::string str;

    for (auto c : v)
        str += c;

    std::cout << str;

    // --- added lines ---
    std::cout << "\nString created using constructor: " << str_v << std::endl;
    // --------------------

    // ---------- MORE ADDED CODE BELOW ----------

    // print size of vector
    std::cout << "Vector size: " << v.size() << std::endl;

    // append another character
    v.push_back('~');
    std::cout << "After adding '~', vector size: " << v.size() << std::endl;

    // print updated vector as string
    std::string updated(v.begin(), v.end());
    std::cout << "Updated string: " << updated << std::endl;

    // reverse print
    std::cout << "Reverse: ";
    for (auto it = v.rbegin(); it != v.rend(); ++it)
        std::cout << *it;
    std::cout << std::endl;

    // ------------------------------------------

    return 0;
}

// ==================================================
// EXTRA CODE (append only)
// ==================================================

// Count vowels in a vector<char>
int count_vowels(const std::vector<char>& v) {
    int count = 0;
    for (char c : v) {
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' ||
            c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U')
            ++count;
    }
    return count;
}

// Convert vector<char> to uppercase string
std::string to_upper_string(const std::vector<char>& v) {
    std::string s;
    for (char c : v) {
        if (c >= 'a' && c <= 'z')
            s += (c - 'a' + 'A');
        else
            s += c;
    }
    return s;
}

// Auto-run extra demo (executes before main)
int __ = []() {
    std::cout << "\n--- Extra Vector<char> Demo ---\n";

    std::vector<char> demo = { 'C', 'p', 'p', '!' };

    std::string s(demo.begin(), demo.end());
    std::cout << "Demo string: " << s << '\n';

    std::cout << "Uppercase: " << to_upper_string(demo) << '\n';
    std::cout << "Vowel count: " << count_vowels(demo) << '\n';

    demo.push_back('?');
    std::cout << "After push_back: ";
    for (char c : demo) std::cout << c;
    std::cout << '\n';

    return 0;
}();
