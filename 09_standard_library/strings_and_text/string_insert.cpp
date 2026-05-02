#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <numeric>   // added

// ======================================================
// ORIGINAL CODE (UNCHANGED LOGIC)
// ======================================================

std::string replace_idx(std::string& s,
                        const std::string& old_val,
                        const std::string& new_val)
{
    auto pos = s.find(old_val);
    if (pos != std::string::npos) {
        s.replace(pos, old_val.size(), new_val);
    }
    return s;
}

// ======================================================
// SMALL EXTRA HELPERS (ADDED ONLY)
// ======================================================

// Convert string to uppercase
std::string to_uppercase(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c) { return std::toupper(c); });
    return s;
}

// Convert string to lowercase
std::string to_lowercase(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return s;
}

// Count characters in a string
std::size_t count_chars(const std::string& s)
{
    return s.size();
}

// Check if substring exists
bool contains(const std::string& s, const std::string& sub)
{
    return s.find(sub) != std::string::npos;
}

// Check if string starts with prefix
bool starts_with(const std::string& s, const std::string& prefix)
{
    return s.rfind(prefix, 0) == 0;
}

// ===== VERY SMALL EXTRA HELPERS =====

// Check if string ends with suffix
bool ends_with(const std::string& s, const std::string& suffix)
{
    if (suffix.size() > s.size()) return false;
    return std::equal(suffix.rbegin(), suffix.rend(), s.rbegin());
}

// Reverse a string
std::string reverse_string(std::string s)
{
    std::reverse(s.begin(), s.end());
    return s;
}

// Count words in a string
int count_words(const std::string& s)
{
    std::stringstream ss(s);
    std::string word;
    int count = 0;
    while (ss >> word) ++count;
    return count;
}

// Tiny divider printer
void print_divider()
{
    std::cout << "---------------------\n";
}

// Safe replace wrapper (uses original function)
std::string safe_replace(std::string& s,
                         const std::string& old_val,
                         const std::string& new_val)
{
    if (!old_val.empty())
        return replace_idx(s, old_val, new_val);
    return s;
}

// ======================================================
// MAIN
// ======================================================

int main() {
    std::string prefix = "mr";
    std::string suffix = "smith";

    std::string concat = prefix + suffix;
    std::cout << concat << std::endl;

    print_divider();

    std::string text = "Hello Mars";
    replace_idx(text, "Mars", "World");
    std::cout << text << '\n';

    // ---- small added usage ----
    std::cout << "Uppercase: " << to_uppercase(text) << '\n';
    std::cout << "Lowercase: " << to_lowercase(text) << '\n';
    std::cout << "Character count: " << count_chars(text) << '\n';

    if (contains(text, "World"))
        std::cout << "Substring \"World\" found\n";

    if (starts_with(text, "Hello"))
        std::cout << "Text starts with \"Hello\"\n";

    if (ends_with(text, "World"))
        std::cout << "Text ends with \"World\"\n";

    safe_replace(text, "World", "C++");
    std::cout << "After safe replace: " << text << '\n';

    // --- very small new demos ---
    std::cout << "Reversed text: " << reverse_string(text) << '\n';
    std::cout << "Word count: " << count_words(text) << '\n';

    print_divider();

    std::cout << "--- End of program ---\n";

    // ===== EXTRA SMALL ADDITIONS (NEW) =====

    // Count vowels
    int vowels = std::count_if(text.begin(), text.end(), [](char c) {
        char l = std::tolower(static_cast<unsigned char>(c));
        return l=='a'||l=='e'||l=='i'||l=='o'||l=='u';
    });
    std::cout << "Vowel count: " << vowels << '\n';

    // ASCII sum of string
    int ascii_sum = std::accumulate(text.begin(), text.end(), 0,
        [](int sum, char c){ return sum + static_cast<int>(c); });
    std::cout << "ASCII sum: " << ascii_sum << '\n';

    // Check if all characters are alphabetic or space
    bool clean = std::all_of(text.begin(), text.end(), [](char c){
        return std::isalpha(static_cast<unsigned char>(c)) || std::isspace(static_cast<unsigned char>(c));
    });
    std::cout << "Only letters/spaces? "
              << (clean ? "Yes" : "No") << '\n';

    // ---- NEW SMALL ADDITIONS ----

    // Count digits in string
    int digits = std::count_if(text.begin(), text.end(), [](char c){
        return std::isdigit(static_cast<unsigned char>(c));
    });
    std::cout << "Digit count: " << digits << '\n';

    // Replace spaces with underscore (copy, original unchanged)
    std::string modified = text;
    std::replace(modified.begin(), modified.end(), ' ', '_');
    std::cout << "Spaces replaced: " << modified << '\n';

    // Find first uppercase character
    auto it = std::find_if(text.begin(), text.end(), [](char c){
        return std::isupper(static_cast<unsigned char>(c));
    });
    if (it != text.end())
        std::cout << "First uppercase: " << *it << '\n';

    // ======================================

    return 0;
}
