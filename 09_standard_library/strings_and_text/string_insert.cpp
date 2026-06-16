#include <iostream>
#include <string>
#include <string_view>
#include <algorithm>
#include <cctype>
#include <ranges>
#include <format>
#include <numeric>
#include <cassert>
#include <optional>
#include <span>

[[nodiscard]] std::string replace_first(std::string s,
                                        std::string_view old_val,
                                        std::string_view new_val) {
    if (old_val.empty()) return s;
    if (const auto pos = s.find(old_val); pos != std::string::npos)
        s.replace(pos, old_val.size(), new_val);
    return s;
}

[[nodiscard]] std::string replace_all(std::string s,
                                      std::string_view old_val,
                                      std::string_view new_val) {
    if (old_val.empty()) return s;
    std::string result;
    result.reserve(s.size());
    std::string::size_type pos = 0, prev = 0;
    while ((pos = s.find(old_val, prev)) != std::string::npos) {
        result.append(s, prev, pos - prev);
        result += new_val;
        prev = pos + old_val.size();
    }
    result.append(s, prev);
    return result;
}

[[nodiscard]] std::string to_uppercase(std::string s) {
    std::ranges::transform(s, s.begin(),
        [](unsigned char c) { return static_cast<char>(std::toupper(c)); });
    return s;
}

[[nodiscard]] std::string to_lowercase(std::string s) {
    std::ranges::transform(s, s.begin(),
        [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return s;
}

[[nodiscard]] std::string reverse_string(std::string s) {
    std::ranges::reverse(s);
    return s;
}

[[nodiscard]] std::string replace_char(std::string s, char from, char to) {
    std::ranges::replace(s, from, to);
    return s;
}

[[nodiscard]] std::string trim(std::string_view sv) {
    const auto start = std::ranges::find_if_not(sv, [](unsigned char c) {
        return std::isspace(c);
    });
    const auto end = std::ranges::find_if_not(sv | std::views::reverse, [](unsigned char c) {
        return std::isspace(c);
    }).base();
    return (start < end) ? std::string(start, end) : std::string{};
}

[[nodiscard]] constexpr bool contains(std::string_view s, std::string_view sub) noexcept {
    return s.find(sub) != std::string_view::npos;
}

[[nodiscard]] constexpr bool starts_with(std::string_view s, std::string_view prefix) noexcept {
    return s.starts_with(prefix);
}

[[nodiscard]] constexpr bool ends_with(std::string_view s, std::string_view suffix) noexcept {
    return s.ends_with(suffix);
}

[[nodiscard]] bool is_palindrome(std::string_view sv) {
    std::string cleaned;
    cleaned.reserve(sv.size());
    for (unsigned char c : sv)
        if (!std::isspace(c))
            cleaned += static_cast<char>(std::tolower(c));
    return std::ranges::equal(cleaned | std::views::take(cleaned.size() / 2),
                              cleaned | std::views::reverse | std::views::take(cleaned.size() / 2));
}

[[nodiscard]] int count_words(std::string_view sv) {
    bool in_word = false;
    int  count   = 0;
    for (unsigned char c : sv) {
        const bool space = std::isspace(c);
        if (!space && !in_word) { ++count; in_word = true; }
        else if (space)           in_word = false;
    }
    return count;
}

[[nodiscard]] int count_vowels(std::string_view sv) {
    constexpr std::string_view vowels = "aeiouAEIOU";
    return static_cast<int>(std::ranges::count_if(sv, [&](char c) {
        return vowels.find(c) != std::string_view::npos;
    }));
}

[[nodiscard]] long ascii_sum(std::string_view sv) noexcept {
    return std::reduce(sv.begin(), sv.end(), 0L,
                       [](long acc, char c) { return acc + static_cast<unsigned char>(c); });
}

[[nodiscard]] std::optional<char> first_uppercase(std::string_view sv) noexcept {
    const auto it = std::ranges::find_if(sv, [](unsigned char c) {
        return std::isupper(c) != 0;
    });
    return (it != sv.end()) ? std::optional{*it} : std::nullopt;
}

struct StringStats {
    std::size_t length;
    int         spaces;
    int         digits;
    int         vowels;
    int         words;
    long        ascii_total;
    bool        all_alpha_or_space;
};

[[nodiscard]] StringStats analyse(std::string_view sv) noexcept {
    return {
        sv.size(),
        static_cast<int>(std::ranges::count_if(sv, [](unsigned char c) { return std::isspace(c) != 0; })),
        static_cast<int>(std::ranges::count_if(sv, [](unsigned char c) { return std::isdigit(c) != 0; })),
        count_vowels(sv),
        count_words(sv),
        ascii_sum(sv),
        std::ranges::all_of(sv, [](unsigned char c) { return std::isalpha(c) != 0 || std::isspace(c) != 0; })
    };
}

void print_divider() {
    std::cout << "---------------------\n";
}

void print_stats(const StringStats& s, std::string_view label = "") {
    if (!label.empty()) std::cout << std::format("\n[Stats: {}]\n", label);
    std::cout << std::format(
        "  length={}, spaces={}, digits={}, vowels={}, words={}, ascii_sum={}, alpha_only={}\n",
        s.length, s.spaces, s.digits, s.vowels, s.words, s.ascii_total,
        s.all_alpha_or_space ? "Yes" : "No");
}

int main() {
    const std::string prefix = "mr";
    const std::string suffix = "smith";

    std::cout << std::format("{}\n", prefix + suffix);
    print_divider();

    std::string text = replace_first("Hello Mars", "Mars", "World");
    std::cout << std::format("{}\n", text);

    std::cout << std::format("Uppercase:  {}\n", to_uppercase(text));
    std::cout << std::format("Lowercase:  {}\n", to_lowercase(text));
    std::cout << std::format("Char count: {}\n", text.size());

    if (contains(text, "World"))   std::cout << "Substring \"World\" found\n";
    if (starts_with(text, "Hello")) std::cout << "Starts with \"Hello\"\n";
    if (ends_with(text, "World"))   std::cout << "Ends with \"World\"\n";

    text = replace_first(text, "World", "C++");
    std::cout << std::format("After replace: {}\n", text);
    std::cout << std::format("Reversed:      {}\n", reverse_string(text));
    std::cout << std::format("Word count:    {}\n", count_words(text));

    print_divider();

    const auto stats = analyse(text);
    print_stats(stats, text);

    std::cout << std::format("Vowel count:        {}\n", stats.vowels);
    std::cout << std::format("ASCII sum:          {}\n", stats.ascii_total);
    std::cout << std::format("Only letters/spaces? {}\n", stats.all_alpha_or_space ? "Yes" : "No");
    std::cout << std::format("Digit count:        {}\n", stats.digits);
    std::cout << std::format("Space count:        {}\n", stats.spaces);

    std::cout << std::format("Spaces->underscore: {}\n", replace_char(text, ' ', '_'));

    if (const auto uc = first_uppercase(text))
        std::cout << std::format("First uppercase: {}\n", *uc);

    std::cout << std::format("Is palindrome? {}\n", is_palindrome(text) ? "Yes" : "No");
    std::cout << std::format("Is palindrome (\"racecar\")? {}\n", is_palindrome("racecar") ? "Yes" : "No");
    std::cout << std::format("String is {}\n", text.empty() ? "empty" : "not empty");
    std::cout << std::format("Appended: {}\n", text + " Demo");

    std::cout << "\n--- Trim demo ---\n";
    const std::string padded = "  hello world  ";
    std::cout << std::format("Trimmed: \"{}\"\n", trim(padded));

    std::cout << "\n--- replace_all demo ---\n";
    std::cout << std::format("{}\n", replace_all("aababab", "ab", "X"));

    assert(to_uppercase("hello") == "HELLO");
    assert(to_lowercase("HELLO") == "hello");
    assert(reverse_string("abc") == "cba");
    assert(starts_with("Hello", "He"));
    assert(ends_with("World", "ld"));
    assert(contains("Hello World", "World"));
    assert(is_palindrome("racecar"));
    assert(!is_palindrome("hello"));
    assert(is_palindrome("A man a plan a canal Panama"));
    assert(count_words("one two three") == 3);
    assert(trim("  hi  ") == "hi");
    assert(replace_all("aababab", "ab", "X") == "aXXX");
    assert(!first_uppercase("hello").has_value());
    assert(first_uppercase("Hello").value() == 'H');

    std::cout << "\nAll assertions passed.\n";
    return 0;
}
