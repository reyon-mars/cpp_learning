#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cctype>
#include <filesystem>
#include <ranges>
#include <span>

namespace fs = std::filesystem;

class file_guard {
    std::fstream handle_;

public:
    explicit file_guard(const fs::path& path)
        : handle_(path, std::ios::in)
    {
        if (!handle_.is_open())
            throw std::runtime_error("Failed to open: " + path.string());
    }

    ~file_guard() = default;

    file_guard(const file_guard&)            = delete;
    file_guard& operator=(const file_guard&) = delete;
    file_guard(file_guard&&)                 = default;
    file_guard& operator=(file_guard&&)      = default;

    [[nodiscard]] std::fstream& get() noexcept { return handle_; }

    void rewind() {
        handle_.clear();
        handle_.seekg(0);
    }
};

[[nodiscard]] std::string read_all(file_guard& fg) {
    fg.rewind();
    return std::string(std::istreambuf_iterator<char>(fg.get()),
                       std::istreambuf_iterator<char>());
}

[[nodiscard]] std::vector<std::string> get_lines(file_guard& fg) {
    fg.rewind();
    std::vector<std::string> lines;
    for (std::string line; std::getline(fg.get(), line);)
        lines.push_back(std::move(line));
    fg.rewind();
    return lines;
}

[[nodiscard]] std::size_t file_size(file_guard& fg) {
    fg.rewind();
    fg.get().seekg(0, std::ios::end);
    auto size = static_cast<std::size_t>(fg.get().tellg());
    fg.rewind();
    return size;
}

[[nodiscard]] bool is_file_empty(file_guard& fg) {
    return file_size(fg) == 0;
}

[[nodiscard]] std::size_t count_lines(file_guard& fg) {
    return get_lines(fg).size();
}

[[nodiscard]] std::size_t count_characters(file_guard& fg) {
    return file_size(fg);
}

[[nodiscard]] std::size_t count_words(file_guard& fg) {
    fg.rewind();
    std::size_t count = 0;
    for (std::string word; fg.get() >> word;) ++count;
    fg.rewind();
    return count;
}

[[nodiscard]] std::size_t count_occurrences(file_guard& fg, std::string_view target) {
    fg.rewind();
    std::size_t count = 0;
    for (std::string word; fg.get() >> word;)
        if (word == target) ++count;
    fg.rewind();
    return count;
}

[[nodiscard]] bool contains_word(file_guard& fg, std::string_view target) {
    return count_occurrences(fg, target) > 0;
}

[[nodiscard]] int count_vowels(file_guard& fg) {
    auto content = read_all(fg);
    constexpr std::string_view vowels = "aeiouAEIOU";
    return static_cast<int>(
        std::ranges::count_if(content, [&](char c) {
            return vowels.find(c) != std::string_view::npos;
        })
    );
}

[[nodiscard]] std::string read_first_line(file_guard& fg) {
    auto lines = get_lines(fg);
    return lines.empty() ? std::string{} : lines.front();
}

[[nodiscard]] std::string read_last_line(file_guard& fg) {
    auto lines = get_lines(fg);
    return lines.empty() ? std::string{} : lines.back();
}

[[nodiscard]] std::size_t longest_line_length(file_guard& fg) {
    auto lines = get_lines(fg);
    if (lines.empty()) return 0;
    return std::ranges::max(lines | std::views::transform(&std::string::size));
}

[[nodiscard]] std::string transform_content(file_guard& fg, auto char_op) {
    auto content = read_all(fg);
    std::ranges::transform(content, content.begin(), char_op);
    return content;
}

[[nodiscard]] std::string to_uppercase(file_guard& fg) {
    return transform_content(fg, [](unsigned char c) {
        return static_cast<char>(std::toupper(c));
    });
}

[[nodiscard]] std::string to_lowercase(file_guard& fg) {
    return transform_content(fg, [](unsigned char c) {
        return static_cast<char>(std::tolower(c));
    });
}

void print_preview(file_guard& fg, std::size_t n) {
    auto content = read_all(fg);
    std::cout << std::string_view(content).substr(0, n) << '\n';
}

void print_numbered_lines(file_guard& fg) {
    auto lines = get_lines(fg);
    for (const auto& [i, line] : lines | std::views::enumerate)
        std::cout << (i + 1) << ": " << line << '\n';
}

void append_to_file(const fs::path& path, std::string_view text) {
    std::ofstream out(path, std::ios::app);
    if (!out)
        throw std::runtime_error("Failed to open for append: " + path.string());
    out << text << '\n';
}

int main() {
    const fs::path filepath = "example.txt";

    if (!fs::exists(filepath)) {
        std::ofstream seed(filepath);
        seed << "Hello world\n"
             << "this is a test file\n"
             << "test line three\n";
    }

    try {
        file_guard fg(filepath);
        std::cout << "File opened successfully.\n\n";

        std::cout << "File contents:\n" << read_all(fg) << '\n';
        std::cout << "Line count:      " << count_lines(fg)      << '\n';
        std::cout << "Word count:      " << count_words(fg)      << '\n';
        std::cout << "File size:       " << file_size(fg)        << " bytes\n";
        std::cout << "Char count:      " << count_characters(fg) << '\n';
        std::cout << "Empty?           " << (is_file_empty(fg) ? "Yes" : "No") << '\n';
        std::cout << "First line:      " << read_first_line(fg)  << '\n';
        std::cout << "Last line:       " << read_last_line(fg)   << '\n';
        std::cout << "Contains 'test'? " << (contains_word(fg, "test") ? "Yes" : "No") << '\n';
        std::cout << "Occurrences of 'test': " << count_occurrences(fg, "test") << '\n';

        std::cout << "\nUppercase:\n"   << to_uppercase(fg) << '\n';
        std::cout << "Lowercase:\n"    << to_lowercase(fg) << '\n';
        std::cout << "Preview (30):\n"; print_preview(fg, 30);

        append_to_file(filepath, "Appended line.");
        std::cout << "\nAppended successfully.\n";

        std::cout << "\n--- Extra File Analysis ---\n";
        std::cout << "Vowel count:         " << count_vowels(fg)         << '\n';
        std::cout << "Longest line length: " << longest_line_length(fg)  << '\n';

        std::cout << "\n--- Numbered Lines ---\n";
        print_numbered_lines(fg);

        std::cout << "\n--- Lines via get_lines() ---\n";
        for (const auto& line : get_lines(fg))
            std::cout << line << '\n';

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
