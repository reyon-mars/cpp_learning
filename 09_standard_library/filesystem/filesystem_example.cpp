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
    
    return 0;
}
