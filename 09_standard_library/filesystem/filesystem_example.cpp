// Filesystem Exercise
// File and directory operations

#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

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

    return 0;
}
