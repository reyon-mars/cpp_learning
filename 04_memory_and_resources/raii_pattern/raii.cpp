#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <sstream>
#include <cctype>   // ✅ ADDED (needed for std::toupper)

// ======================================================
// ORIGINAL CLASS (UNCHANGED LOGIC)
// ======================================================

class file_guard {
private:
    std::fstream f_handle;

public:
    explicit file_guard(const std::string& filename)
        : f_handle(filename, std::ios::in)
    {
        if (!f_handle.is_open()) {
            throw std::runtime_error("Failed to open " + filename);
        }
    }

    ~file_guard() {
        if (f_handle.is_open()) {
            f_handle.close();
        }
    }

    file_guard(const file_guard&) = delete;
    file_guard& operator=(const file_guard&) = delete;

    std::fstream& get() { return f_handle; }
};

// ======================================================
// SMALL EXTRA CODE (ADDED ONLY)
// ======================================================

// Check if file exists
bool file_exists(const std::string& filename) {
    std::ifstream f(filename);
    return f.good();
}

// Reset file stream to beginning
void rewind_file(file_guard& fg) {
    auto& f = fg.get();
    f.clear();
    f.seekg(0);
}

// Read entire file
std::string read_all(file_guard& fg) {
    rewind_file(fg);

    std::string content, line;
    while (std::getline(fg.get(), line)) {
        content += line + '\n';
    }
    return content;
}

// Count number of lines
int count_lines(file_guard& fg) {
    rewind_file(fg);

    int count = 0;
    std::string line;
    while (std::getline(fg.get(), line))
        ++count;

    return count;
}

// Read first line
std::string read_first_line(file_guard& fg) {
    rewind_file(fg);
    std::string line;
    std::getline(fg.get(), line);
    return line;
}

// Print preview (first N characters)
void print_preview(file_guard& fg, std::size_t n) {
    rewind_file(fg);

    char ch;
    std::size_t count = 0;
    while (fg.get().get(ch) && count < n) {
        std::cout << ch;
        ++count;
    }
    std::cout << '\n';
}

// ---------------- NEW SMALL ADDITIONS ----------------

// Get file size
std::size_t file_size(file_guard& fg) {
    rewind_file(fg);
    auto& f = fg.get();

    f.seekg(0, std::ios::end);
    std::size_t size = f.tellg();
    rewind_file(fg);
    return size;
}

// Count words
int count_words(file_guard& fg) {
    rewind_file(fg);

    int count = 0;
    std::string word;
    while (fg.get() >> word)
        ++count;

    rewind_file(fg);
    return count;
}

// Append text to file
void append_to_file(const std::string& filename,
                    const std::string& text) {
    std::ofstream out(filename, std::ios::app);
    if (out.is_open()) {
        out << text << '\n';
    }
}

// -------- MORE ADDITIONS --------

// Check if file is empty
bool is_file_empty(file_guard& fg) {
    return file_size(fg) == 0;
}

// Count characters
std::size_t count_characters(file_guard& fg) {
    rewind_file(fg);

    std::size_t count = 0;
    char ch;
    while (fg.get().get(ch))
        ++count;

    rewind_file(fg);
    return count;
}

// Search for a word
bool contains_word(file_guard& fg, const std::string& target) {
    rewind_file(fg);

    std::string word;
    while (fg.get() >> word) {
        if (word == target)
            return true;
    }

    rewind_file(fg);
    return false;
}

// Read last line
std::string read_last_line(file_guard& fg) {
    rewind_file(fg);

    std::string line, last;
    while (std::getline(fg.get(), line)) {
        last = line;
    }
    return last;
}

// Safe append with message
void append_with_status(const std::string& filename,
                        const std::string& text) {
    std::ofstream out(filename, std::ios::app);
    if (out.is_open()) {
        out << text << '\n';
        std::cout << "Append successful\n";
    } else {
        std::cout << "Append failed\n";
    }
}

// -------- EXTRA SMALL ADDITIONS --------

// Count occurrences of a specific word
int count_occurrences(file_guard& fg, const std::string& target) {
    rewind_file(fg);

    int count = 0;
    std::string word;
    while (fg.get() >> word) {
        if (word == target)
            ++count;
    }

    rewind_file(fg);
    return count;
}

// Convert file content to uppercase
std::string to_uppercase(file_guard& fg) {
    rewind_file(fg);

    std::string content, line;
    while (std::getline(fg.get(), line)) {
        for (char& c : line)
            c = std::toupper(c);
        content += line + '\n';
    }

    rewind_file(fg);
    return content;
}

// ======================================================
// MAIN
// ======================================================

int main() {
    const std::string filename = "example.txt";

    if (!file_exists(filename)) {
        std::cout << "File does not exist: " << filename << '\n';
        return 0;
    }

    try {
        file_guard fg(filename);

        std::cout << "File opened successfully.\n\n";

        std::cout << "File contents:\n";
        std::cout << read_all(fg) << '\n';

        std::cout << "Line count: "
                  << count_lines(fg) << '\n';

        std::cout << "Word count: "
                  << count_words(fg) << '\n';

        std::cout << "File size (bytes): "
                  << file_size(fg) << '\n';

        std::cout << "Character count: "
                  << count_characters(fg) << '\n';

        std::cout << "File empty? "
                  << (is_file_empty(fg) ? "Yes\n" : "No\n");

        std::cout << "First line: "
                  << read_first_line(fg) << '\n';

        std::cout << "Last line: "
                  << read_last_line(fg) << '\n';

        std::cout << "Contains word 'test'? "
                  << (contains_word(fg, "test") ? "Yes\n" : "No\n");

        std::cout << "Occurrences of 'test': "
                  << count_occurrences(fg, "test") << '\n';

        std::cout << "\nUppercase preview:\n";
        std::cout << to_uppercase(fg) << '\n';

        std::cout << "Preview (30 chars):\n";
        print_preview(fg, 30);

        append_with_status(filename, "Appended line.");

        std::cout << "\nLine count after append (reopen to verify): "
                  << count_lines(fg) << '\n';

    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << '\n';
    }

    return 0;
}
