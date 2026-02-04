#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>

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

// ====================================

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
    std::cout << text << '\n';  // Hello World

    // ---- small added usage ----
    std::string upper = to_uppercase(text);
    std::cout << "Uppercase: " << upper << '\n';

    std::string lower = to_lowercase(text);
    std::cout << "Lowercase: " << lower << '\n';

    std::cout << "Character count: "
              << count_chars(text) << '\n';

    if (contains(text, "World")) {
        std::cout << "Substring \"World\" found\n";
    }

    if (starts_with(text, "Hello")) {
        std::cout << "Text starts with \"Hello\"\n";
    }

    // ---- VERY SMALL EXTRA USAGE ----
    if (ends_with(text, "World")) {
        std::cout << "Text ends with \"World\"\n";
    }

    safe_replace(text, "World", "C++");
    std::cout << "After safe replace: " << text << '\n';
    // --------------------------------

    print_divider();

    std::cout << "\n--- End of main() ---\n";
    return 0;
}
