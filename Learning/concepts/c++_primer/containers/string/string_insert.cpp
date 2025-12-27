#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>

// ======================================================
// ORIGINAL CODE (UNCHANGED)
// ======================================================

std::string replace_idx( std::string& s,
                         const std::string& old_val,
                         const std::string& new_val )
{
    auto pos = s.find(old_val);
    if (pos != std::string::npos) {
        s.replace(pos, old_val.size(), new_val);
    }
    return s;
}

int main() {
    std::string prefix = "mr";
    std::string suffix = "smith";

    std::string concat = prefix + suffix;
    std::cout << concat << std::endl;

    std::string text = "Hello Mars";
    replace_idx(text, "Mars", "World");
    std::cout << text << '\n';  // Output: Hello World

    std::cout << "\n--- End of main() ---\n";
    return 0;
}

// ======================================================
// EXTRA CODE ADDED BELOW (original code unchanged)
// ======================================================

// Replace all occurrences of a substring
std::string replace_all(std::string s,
                        const std::string& old_val,
                        const std::string& new_val)
{
    size_t pos = 0;
    while ((pos = s.find(old_val, pos)) != std::string::npos) {
        s.replace(pos, old_val.size(), new_val);
        pos += new_val.size();
    }
    return s;
}

void printInfo() {
    std::cout << "[Extra] Replace utility running!\n";
}

// Auto-run demo using global lambda
int runExtra = []() {
    printInfo();
    std::string test = "foo bar foo bar foo";
    std::cout << "[Extra] Before: " << test << '\n';
    test = replace_all(test, "foo", "baz");
    std::cout << "[Extra] After : " << test << '\n';
    return 0;
}();

// ======================================================
// MORE EXTRA CODE ADDED (still untouched above)
// ======================================================

// Count occurrences of a substring
int count_occurrences(const std::string& s, const std::string& sub)
{
    int count = 0;
    size_t pos = 0;
    while ((pos = s.find(sub, pos)) != std::string::npos) {
        ++count;
        pos += sub.size();
    }
    return count;
}

// Convert string to uppercase
std::string to_uppercase(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c) { return std::toupper(c); });
    return s;
}

// Reverse a string
std::string reverse_string(std::string s)
{
    std::reverse(s.begin(), s.end());
    return s;
}

// Auto-run another demo
int __ = []() {
    std::cout << "\n[More Extra]\n";

    std::string sample = "one fish two fish red fish blue fish";
    std::cout << "Sample text: " << sample << '\n';

    std::cout << "Occurrences of 'fish': "
              << count_occurrences(sample, "fish") << '\n';

    std::string modified = replace_all(sample, "fish", "cat");
    std::cout << "After replace_all: " << modified << '\n';

    std::cout << "Uppercase: " << to_uppercase(modified) << '\n';
    std::cout << "Reversed : " << reverse_string(modified) << '\n';

    return 0;
}();
