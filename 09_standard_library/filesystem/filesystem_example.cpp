// Filesystem Exercise
// File and directory operations

#include <iostream>
#include <filesystem>
#include <vector>      // 🔹 added
#include <algorithm>   // 🔹 added

namespace fs = std::filesystem;

// ---------------- SMALL EXTRA HELPERS ----------------

// Print divider
void print_divider() {
    std::cout << "-----------------------------\n";
}

// Check if file has extension
bool has_txt_extension(const fs::path& p) {
    return p.extension() == ".txt";
}

// Print path components
void print_components(const fs::path& p) {
    std::cout << "Path components:\n";
    for (const auto& part : p) {
        std::cout << part << "\n";
    }
}

// =====================================================

int main() {
    // Create path
    fs::path file_path = "/tmp/test.txt";
    
    // Get file information
    std::cout << "Path: " << file_path << "\n";
    std::cout << "Filename: " << file_path.filename() << "\n";
    std::cout << "Extension: " << file_path.extension() << "\n";
    
    // Check if path exists
    if (fs::exists(file_path)) {
        std::cout << "File exists\n";
        std::cout << "Is regular file: " << fs::is_regular_file(file_path) << "\n";
        std::cout << "Size: " << fs::file_size(file_path) << " bytes\n";
    }

    // ---- Additional small examples ----

    // current working directory
    std::cout << "Current path: " << fs::current_path() << "\n";

    // parent directory
    std::cout << "Parent path: " << file_path.parent_path() << "\n";

    // check if directory
    fs::path dir = "/tmp";
    if (fs::exists(dir) && fs::is_directory(dir)) {
        std::cout << "Listing files in /tmp:\n";
        for (const auto& entry : fs::directory_iterator(dir)) {
            std::cout << entry.path() << "\n";
        }
    }

    // ---- VERY SMALL EXTRA ADDITIONS ----

    // absolute path
    std::cout << "Absolute path: " << fs::absolute(file_path) << "\n";

    // file stem (name without extension)
    std::cout << "Stem: " << file_path.stem() << "\n";

    // check empty path
    std::cout << "Is path empty? "
              << (file_path.empty() ? "Yes" : "No") << "\n";

    // temp directory path
    std::cout << "Temp directory: " << fs::temp_directory_path() << "\n";

    // compare paths
    fs::path another = "/tmp/test.txt";
    std::cout << "Paths equal? "
              << (file_path == another ? "Yes" : "No") << "\n";

    // -----------------------------------
    // EXTRA SMALL ADDITIONS (NO CHANGE TO ORIGINAL LOGIC)
    // -----------------------------------

    // Check if path is absolute or relative
    std::cout << "Is absolute? "
              << (file_path.is_absolute() ? "Yes" : "No") << "\n";

    // Replace filename (no actual file change, just path object)
    fs::path modified = file_path;
    modified.replace_filename("new_test.txt");
    std::cout << "Modified filename path: " << modified << "\n";

    // Replace extension
    modified.replace_extension(".log");
    std::cout << "After changing extension: " << modified << "\n";

    // Check if two paths are equivalent (only if both exist)
    if (fs::exists(file_path) && fs::exists(another)) {
        std::cout << "Equivalent paths? "
                  << (fs::equivalent(file_path, another) ? "Yes" : "No") << "\n";
    }

    // File status check
    std::cout << "Exists (again check): "
              << (fs::exists(file_path) ? "Yes" : "No") << "\n";

    // ==================================================
    // 🔹 NEW SMALL ADDITIONS
    // ==================================================

    print_divider();

    // Check root path
    std::cout << "Root path: "
              << file_path.root_path() << "\n";

    // Relative path
    std::cout << "Relative path: "
              << file_path.relative_path() << "\n";

    // Check extension helper
    std::cout << "Has .txt extension? "
              << (has_txt_extension(file_path) ? "Yes" : "No") << "\n";

    // Print path components
    print_components(file_path);

    // Lexically normal path
    fs::path messy = "/tmp/../tmp/test.txt";
    std::cout << "Normalized path: "
              << messy.lexically_normal() << "\n";

    // Copy path into vector
    std::vector<fs::path> paths = {
        file_path,
        modified,
        messy
    };

    std::cout << "Stored paths:\n";
    for (const auto& p : paths) {
        std::cout << p << "\n";
    }

    // Sort paths alphabetically
    std::sort(paths.begin(), paths.end());

    std::cout << "Sorted paths:\n";
    for (const auto& p : paths) {
        std::cout << p << "\n";
    }

    // ==================================================

    return 0;
}
