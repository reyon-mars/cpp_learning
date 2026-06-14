#include <iostream>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <string_view>

namespace fs = std::filesystem;

void print_path_info(const fs::path& p, std::string_view label = "") {
    if (!label.empty()) std::cout << label << "\n";
    std::cout << "  full="      << p                   << "\n"
              << "  filename="  << p.filename()         << "\n"
              << "  stem="      << p.stem()             << "\n"
              << "  extension=" << p.extension()        << "\n"
              << "  parent="    << p.parent_path()      << "\n"
              << "  root="      << p.root_path()        << "\n"
              << "  relative="  << p.relative_path()    << "\n"
              << "  absolute="  << fs::absolute(p)      << "\n"
              << "  is_absolute=" << std::boolalpha << p.is_absolute() << "\n"
              << "  empty="       << p.empty()          << "\n";
}

void print_components(const fs::path& p) {
    std::cout << "Components of " << p << ":\n";
    for (const auto& part : p) std::cout << "  " << part << "\n";
}

[[nodiscard]] bool has_extension(const fs::path& p, std::string_view ext) {
    return p.extension() == ext;
}

void print_file_status(const fs::path& p) {
    if (!fs::exists(p)) { std::cout << p << " does not exist\n"; return; }
    std::cout << p << ":\n"
              << "  is_regular_file=" << fs::is_regular_file(p) << "\n"
              << "  is_directory="    << fs::is_directory(p)    << "\n";
    if (fs::is_regular_file(p))
        std::cout << "  size=" << fs::file_size(p) << " bytes\n";
}

int main() {
    const fs::path file_path{"/tmp/test.txt"};
    const fs::path dir_path{"/tmp"};

    std::cout << "=== Path decomposition ===\n";
    print_path_info(file_path, "file_path");

    std::cout << "\n=== File status ===\n";
    print_file_status(file_path);

    std::cout << "\n=== Directory ===\n";
    std::cout << "cwd=" << fs::current_path() << "\n"
              << "tmp=" << fs::temp_directory_path() << "\n";

    if (fs::exists(dir_path) && fs::is_directory(dir_path)) {
        std::cout << "\nListing /tmp:\n";
        for (const auto& entry : fs::directory_iterator{dir_path})
            std::cout << "  " << entry.path() << "\n";
    }

    std::cout << "\n=== Path manipulation ===\n";
    fs::path modified = file_path;
    modified.replace_filename("new_test.txt");
    std::cout << "replace_filename: " << modified << "\n";
    modified.replace_extension(".log");
    std::cout << "replace_extension: " << modified << "\n";

    const fs::path messy{"/tmp/../tmp/test.txt"};
    std::cout << "lexically_normal: " << messy.lexically_normal() << "\n";

    std::cout << "\n=== Path comparison ===\n";
    const fs::path other{"/tmp/test.txt"};
    std::cout << "file_path == other: " << (file_path == other) << "\n";
    if (fs::exists(file_path) && fs::exists(other))
        std::cout << "equivalent: " << fs::equivalent(file_path, other) << "\n";

    std::cout << "\n=== Extension helper ===\n";
    std::cout << "has .txt: " << has_extension(file_path, ".txt") << "\n"
              << "has .log: " << has_extension(modified, ".log")  << "\n";

    std::cout << "\n=== Components ===\n";
    print_components(file_path);

    std::cout << "\n=== Vector of paths ===\n";
    std::vector<fs::path> paths{file_path, modified, messy};
    std::ranges::sort(paths);
    for (const auto& p : paths) std::cout << "  " << p << "\n";

    return 0;
}
