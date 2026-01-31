#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

// ======================================================
// ORIGINAL CLASS (UNCHANGED)
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

        std::cout << "First line: "
                  << read_first_line(fg) << '\n';

        std::cout << "Preview (30 chars):\n";
        print_preview(fg, 30);

    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << '\n';
    }

    return 0;
}
