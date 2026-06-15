#include <iostream>
#include <vector>
#include <string>
#include <string_view>
#include <algorithm>
#include <cctype>
#include <numeric>
#include <span>
#include <ranges>

[[nodiscard]] bool is_vowel(char c) noexcept {
    c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
}

void print_chars(std::span<const char> v, std::string_view label = "") {
    if (!label.empty()) std::cout << label << ": ";
    for (char c : v) std::cout << c;
    std::cout << "\n";
}

[[nodiscard]] long count_vowels(std::span<const char> v) {
    return std::ranges::count_if(v, is_vowel);
}

[[nodiscard]] long count_alpha(std::span<const char> v) {
    return std::ranges::count_if(v, [](char c){
        return std::isalpha(static_cast<unsigned char>(c)) != 0;
    });
}

[[nodiscard]] int ascii_sum(std::span<const char> v) noexcept {
    return std::accumulate(v.begin(), v.end(), 0,
        [](int s, char c){ return s + static_cast<int>(c); });
}

[[nodiscard]] bool all_printable(std::span<const char> v) noexcept {
    return std::ranges::all_of(v, [](char c){
        return std::isprint(static_cast<unsigned char>(c)) != 0;
    });
}

[[nodiscard]] bool is_palindrome(std::span<const char> v) noexcept {
    return std::equal(v.begin(), v.begin() + v.size() / 2, v.rbegin());
}

[[nodiscard]] std::string to_string(std::span<const char> v) {
    return {v.begin(), v.end()};
}

int main() {
    std::vector<char> v;
    v.reserve(10);
    v.push_back('H');
    v.push_back('i');
    v.push_back('!');

    const std::string str = to_string(v);
    std::cout << "String: " << str << "\n"
              << "size="     << v.size()     << "\n"
              << "capacity=" << v.capacity() << "\n";

    if (!v.empty())
        std::cout << "front=" << v.front() << " back=" << v.back() << "\n";

    std::cout << std::boolalpha
              << "ASCII(front)="   << static_cast<int>(v.front())                        << "\n"
              << "contains('!')="  << (std::ranges::find(v, '!') != v.end())            << "\n"
              << "count('i')="     << std::ranges::count(v, 'i')                         << "\n"
              << "count_vowels="   << count_vowels(v)                                    << "\n"
              << "count_alpha="    << count_alpha(v)                                     << "\n"
              << "ascii_sum="      << ascii_sum(v)                                       << "\n"
              << "all_printable="  << all_printable(v)                                   << "\n"
              << "is_palindrome="  << is_palindrome(v)                                   << "\n";

    std::cout << "\n=== Views (no copy) ===\n";
    std::cout << "Reversed: ";
    for (char c : v | std::views::reverse) std::cout << c;
    std::cout << "\n";

    std::cout << "Uppercase: ";
    for (char c : v | std::views::transform([](char c){
            return static_cast<char>(std::toupper(static_cast<unsigned char>(c))); }))
        std::cout << c;
    std::cout << "\n";

    std::cout << "\n=== Copies + mutations ===\n";
    auto sorted_v = v;
    std::ranges::sort(sorted_v);
    print_chars(sorted_v, "sorted");

    auto reversed_v = v;
    std::ranges::reverse(reversed_v);
    print_chars(reversed_v, "reversed copy");

    std::vector<char> letters_only;
    std::ranges::copy_if(v, std::back_inserter(letters_only), [](char c){
        return std::isalpha(static_cast<unsigned char>(c)) != 0;
    });
    print_chars(letters_only, "letters only");

    std::string lower = str;
    std::ranges::transform(lower, lower.begin(), [](char c){
        return static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    });
    std::cout << "lowercase=" << lower << "\n";

    v.push_back('?');
    print_chars(v, "after push_back('?')");

    return 0;
}
