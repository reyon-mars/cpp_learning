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
#include <vector>
#include <map>
#include <sstream>
#include <charconv>
#include <system_error>

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
    const auto start = std::ranges::find_if_not(sv,
        [](unsigned char c) { return std::isspace(c); });
    const auto end = std::ranges::find_if_not(sv | std::views::reverse,
        [](unsigned char c) { return std::isspace(c); }).base();
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
    return std::ranges::equal(
        cleaned | std::views::take(cleaned.size() / 2),
        cleaned | std::views::reverse | std::views::take(cleaned.size() / 2));
}

[[nodiscard]] int count_words(std::string_view sv) {
    bool in_word = false;
    int  count   = 0;
    for (unsigned char c : sv) {
        if (!std::isspace(c) && !in_word) { ++count; in_word = true; }
        else if (std::isspace(c))           in_word = false;
    }
    return count;
}

[[nodiscard]] int count_vowels(std::string_view sv) {
    constexpr std::string_view vowels = "aeiouAEIOU";
    return static_cast<int>(std::ranges::count_if(sv,
        [&](char c) { return vowels.find(c) != std::string_view::npos; }));
}

[[nodiscard]] long ascii_sum(std::string_view sv) noexcept {
    return std::reduce(sv.begin(), sv.end(), 0L,
        [](long acc, char c) { return acc + static_cast<unsigned char>(c); });
}

[[nodiscard]] std::optional<char> first_uppercase(std::string_view sv) noexcept {
    const auto it = std::ranges::find_if(sv,
        [](unsigned char c) { return std::isupper(c) != 0; });
    return (it != sv.end()) ? std::optional{*it} : std::nullopt;
}

[[nodiscard]] std::vector<std::string> split(std::string_view sv, char delim) {
    std::vector<std::string> tokens;
    std::string::size_type prev = 0;
    while (prev < sv.size()) {
        const auto pos = sv.find(delim, prev);
        const auto end = (pos == std::string_view::npos) ? sv.size() : pos;
        tokens.emplace_back(sv.substr(prev, end - prev));
        if (pos == std::string_view::npos) break;
        prev = pos + 1;
    }
    return tokens;
}

[[nodiscard]] std::string join(std::span<const std::string> parts, std::string_view sep) {
    if (parts.empty()) return {};
    std::string result = parts[0];
    for (std::size_t i = 1; i < parts.size(); ++i) {
        result += sep;
        result += parts[i];
    }
    return result;
}

[[nodiscard]] std::string caesar_cipher(std::string_view sv, int shift) {
    shift = ((shift % 26) + 26) % 26;
    std::string result(sv.size(), '\0');
    std::ranges::transform(sv, result.begin(), [shift](unsigned char c) -> char {
        if (std::isalpha(c)) {
            const char base = std::isupper(c) ? 'A' : 'a';
            return static_cast<char>(base + (c - base + shift) % 26);
        }
        return static_cast<char>(c);
    });
    return result;
}

[[nodiscard]] std::string rot13(std::string_view sv) {
    return caesar_cipher(sv, 13);
}

[[nodiscard]] std::map<char, int> char_frequency(std::string_view sv) {
    std::map<char, int> freq;
    for (const char c : sv)
        if (!std::isspace(static_cast<unsigned char>(c)))
            ++freq[c];
    return freq;
}

[[nodiscard]] std::optional<int> parse_int(std::string_view sv) noexcept {
    int result{};
    const auto [ptr, ec] = std::from_chars(sv.data(), sv.data() + sv.size(), result);
    if (ec == std::errc{} && ptr == sv.data() + sv.size())
        return result;
    return std::nullopt;
}

[[nodiscard]] std::string repeat(std::string_view sv, int n) {
    if (n <= 0) return {};
    std::string result;
    result.reserve(sv.size() * static_cast<std::size_t>(n));
    for (int i = 0; i < n; ++i) result += sv;
    return result;
}

[[nodiscard]] std::string title_case(std::string s) {
    bool next_upper = true;
    for (unsigned char& c : reinterpret_cast<std::vector<unsigned char>&>(s)) {
        if (std::isspace(c)) { next_upper = true; }
        else if (next_upper) { c = static_cast<unsigned char>(std::toupper(c)); next_upper = false; }
        else                 { c = static_cast<unsigned char>(std::tolower(c)); }
    }
    return s;
}

[[nodiscard]] std::string title_case_v2(std::string s) {
    bool next_upper = true;
    for (char& ch : s) {
        unsigned char uc = static_cast<unsigned char>(ch);
        if (std::isspace(uc)) { next_upper = true; }
        else if (next_upper)  { ch = static_cast<char>(std::toupper(uc)); next_upper = false; }
        else                  { ch = static_cast<char>(std::tolower(uc)); }
    }
    return s;
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
        static_cast<int>(std::ranges::count_if(sv, [](unsigned char c){ return std::isspace(c)  != 0; })),
        static_cast<int>(std::ranges::count_if(sv, [](unsigned char c){ return std::isdigit(c)  != 0; })),
        count_vowels(sv),
        count_words(sv),
        ascii_sum(sv),
        std::ranges::all_of(sv, [](unsigned char c){ return std::isalpha(c) != 0 || std::isspace(c) != 0; })
    };
}

void print_divider() { std::cout << "---------------------\n"; }

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

    if (contains(text, "World"))    std::cout << "Substring \"World\" found\n";
    if (starts_with(text, "Hello")) std::cout << "Starts with \"Hello\"\n";
    if (ends_with(text, "World"))   std::cout << "Ends with \"World\"\n";

    text = replace_first(text, "World", "C++");
    std::cout << std::format("After replace: {}\n", text);
    std::cout << std::format("Reversed:      {}\n", reverse_string(text));
    std::cout << std::format("Word count:    {}\n", count_words(text));

    print_divider();
    const auto stats = analyse(text);
    print_stats(stats, text);
    std::cout << std::format("Vowel count:          {}\n", stats.vowels);
    std::cout << std::format("ASCII sum:            {}\n", stats.ascii_total);
    std::cout << std::format("Only letters/spaces?  {}\n", stats.all_alpha_or_space ? "Yes" : "No");
    std::cout << std::format("Digit count:          {}\n", stats.digits);
    std::cout << std::format("Space count:          {}\n", stats.spaces);
    std::cout << std::format("Spaces->underscore:   {}\n", replace_char(text, ' ', '_'));

    if (const auto uc = first_uppercase(text))
        std::cout << std::format("First uppercase: {}\n", *uc);

    std::cout << std::format("Is palindrome? {}\n",          is_palindrome(text) ? "Yes" : "No");
    std::cout << std::format("Is palindrome (racecar)? {}\n",is_palindrome("racecar") ? "Yes" : "No");
    std::cout << std::format("String is {}\n", text.empty() ? "empty" : "not empty");
    std::cout << std::format("Appended: {}\n", text + " Demo");

    std::cout << "\n--- Trim demo ---\n";
    std::cout << std::format("Trimmed: \"{}\"\n", trim("  hello world  "));

    std::cout << "\n--- replace_all demo ---\n";
    std::cout << std::format("{}\n", replace_all("aababab", "ab", "X"));

    std::cout << "\n--- Split and join ---\n";
    const auto tokens = split("one,two,three,four", ',');
    for (const auto& tok : tokens) std::cout << std::format("  token: {}\n", tok);
    std::cout << std::format("Joined: {}\n", join(tokens, " | "));
    std::cout << std::format("Joined (space): {}\n", join(tokens, " "));

    std::cout << "\n--- Caesar cipher / ROT13 ---\n";
    constexpr std::string_view plain = "Hello World";
    const std::string encrypted = caesar_cipher(plain, 3);
    const std::string decrypted = caesar_cipher(encrypted, -3);
    std::cout << std::format("Plain:     {}\n", plain);
    std::cout << std::format("Caesar+3:  {}\n", encrypted);
    std::cout << std::format("Decrypted: {}\n", decrypted);
    std::cout << std::format("ROT13:     {}\n", rot13(plain));
    std::cout << std::format("ROT13x2:   {}\n", rot13(rot13(plain)));

    std::cout << "\n--- Character frequency ---\n";
    const auto freq = char_frequency("hello world");
    for (const auto& [ch, cnt] : freq)
        std::cout << std::format("  '{}': {}\n", ch, cnt);

    std::cout << "\n--- parse_int (std::from_chars) ---\n";
    for (std::string_view sv : {"42", "-7", "0", "abc", "99z"}) {
        if (const auto n = parse_int(sv))
            std::cout << std::format("  \"{}\" -> {}\n", sv, *n);
        else
            std::cout << std::format("  \"{}\" -> invalid\n", sv);
    }

    std::cout << "\n--- repeat ---\n";
    std::cout << std::format("repeat('ab', 4): {}\n", repeat("ab", 4));
    std::cout << std::format("repeat('-', 20): {}\n", repeat("-", 20));

    std::cout << "\n--- title_case ---\n";
    std::cout << std::format("title: {}\n", title_case_v2("the quick brown fox"));

    std::cout << "\n--- std::format templating ---\n";
    const auto make_greeting = [](std::string_view name, int age) {
        return std::format("Hello, {}! You are {} years old.", name, age);
    };
    std::cout << std::format("{}\n", make_greeting("Alice", 30));
    std::cout << std::format("{}\n", make_greeting("Bob",   25));

    std::cout << "\n--- Assertions ---\n";
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
    assert(split("a,b,c", ',') == (std::vector<std::string>{"a", "b", "c"}));
    assert(join(std::vector<std::string>{"a", "b", "c"}, "-") == "a-b-c");
    assert(caesar_cipher("abc", 1) == "bcd");
    assert(caesar_cipher("xyz", 3) == "abc");
    assert(rot13(rot13("Hello")) == "Hello");
    assert(parse_int("42").value() == 42);
    assert(!parse_int("abc").has_value());
    assert(repeat("ab", 3) == "ababab");
    assert(repeat("x", 0).empty());
    assert(title_case_v2("hello world") == "Hello World");

    std::cout << "All assertions passed.\n";
    return 0;
}
