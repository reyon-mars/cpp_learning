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

// Read entire file safely
std::string read_all(file_guard& fg) {
    std::string content, line;
    auto& f = fg.get();

    while (std::getline(f, line)) {
        content += line + '\n';
    }
    return content;
}

// Simple RAII scope logger
struct ScopeLogger {
    std::string name;

    explicit ScopeLogger(std::string n) : name(std::move(n)) {
        std::cout << "[Scope] Enter " << name << '\n';
    }

    ~ScopeLogger() {
        std::cout << "[Scope] Exit " << name << '\n';
    }
};

// Reset file stream to beginning
void rewind_file(file_guard& fg) {
    auto& f = fg.get();
    f.clear();
    f.seekg(0);
}

// Count number of lines in file
int count_lines(file_guard& fg) {
    rewind_file(fg);

    int count = 0;
    std::string line;
    auto& f = fg.get();

    while (std::getline(f, line))
        ++count;

    return count;
}

// ======================================================
// MAIN
// ======================================================

int main(void) {
    const std::string filename = "example.txt";

    if (!file_exists(filename)) {
        std::cout << "File does not exist: " << filename << '\n';
        return 0;
    }

    try {
        ScopeLogger scope("file_guard demo");

        file_guard fg(filename);

        std::cout << "File opened successfully.\n";
        std::cout << "File contents:\n";
        std::cout << read_all(fg);

        // ---- small added usage ----
        std::cout << "\nLine count: "
                  << count_lines(fg) << '\n';

    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << '\n';
    }

    return 0;
}
