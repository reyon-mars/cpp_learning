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

    // -----------------------------------

    return 0;
}
