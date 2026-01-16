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

// Count characters in a string
std::size_t count_chars(const std::string& s)
{
    return s.size();
}

// ======================================================
// MAIN
// ======================================================

int main() {
    std::string prefix = "mr";
    std::string suffix = "smith";

    std::string concat = prefix + suffix;
    std::cout << concat << std::endl;

    std::string text = "Hello Mars";
    replace_idx(text, "Mars", "World");
    std::cout << text << '\n';  // Hello World

    // ---- small added usage ----
    std::string upper = to_uppercase(text);
    std::cout << "Uppercase: " << upper << '\n';

    std::cout << "Character count: "
              << count_chars(text) << '\n';
    // ---------------------------

    std::cout << "\n--- End of main() ---\n";
    return 0;
}
