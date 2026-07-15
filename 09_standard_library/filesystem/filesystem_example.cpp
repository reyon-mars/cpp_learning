#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <string_view>
#include <system_error>
#include <chrono>
#include <iomanip>
#include <ctime>

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

void write_text_file(const fs::path& p, std::string_view content) {
    std::ofstream out{p, std::ios::binary};
    out << content;
}

[[nodiscard]] std::string permissions_to_string(fs::perms p) {
    auto bit = [p](fs::perms flag, char c) {
        return (p & flag) != fs::perms::none ? c : '-';
    };
    std::string result;
    result += bit(fs::perms::owner_read, 'r');
    result += bit(fs::perms::owner_write, 'w');
    result += bit(fs::perms::owner_exec, 'x');
    result += bit(fs::perms::group_read, 'r');
    result += bit(fs::perms::group_write, 'w');
    result += bit(fs::perms::group_exec, 'x');
    result += bit(fs::perms::others_read, 'r');
    result += bit(fs::perms::others_write, 'w');
    result += bit(fs::perms::others_exec, 'x');
    return result;
}

void print_last_write_time(const fs::path& p) {
    std::error_code ec;
    auto ftime = fs::last_write_time(p, ec);
    if (ec) { std::cout << "  last_write_time unavailable: " << ec.message() << "\n"; return; }
    auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
        ftime - fs::file_time_type::clock::now() + std::chrono::system_clock::now());
    auto tt = std::chrono::system_clock::to_time_t(sctp);
    std::cout << "  last_write_time=" << std::put_time(std::localtime(&tt), "%F %T") << "\n";
}

void demonstrate_directory_tree(const fs::path& root) {
    std::error_code ec;
    fs::remove_all(root, ec);
    fs::create_directories(root / "nested" / "deeper", ec);
    write_text_file(root / "alpha.txt", "alpha contents");
    write_text_file(root / "nested" / "beta.log", "beta contents");
    write_text_file(root / "nested" / "deeper" / "gamma.txt", "gamma contents");

    std::cout << "Flat listing of " << root << ":\n";
    for (const auto& entry : fs::directory_iterator{root})
        std::cout << "  " << entry.path().filename() << (entry.is_directory() ? "/" : "") << "\n";

    std::cout << "Recursive listing of " << root << ":\n";
    for (const auto& entry : fs::recursive_directory_iterator{root})
        std::cout << "  " << entry.path().lexically_relative(root) << "\n";
}

int main() {
    const fs::path file_path{"/tmp/test.txt"};
    const fs::path dir_path{"/tmp"};

    write_text_file(file_path, "hello filesystem");

    std::cout << "=== Path decomposition ===\n";
    print_path_info(file_path, "file_path");

    std::cout << "\n=== File status ===\n";
    print_file_status(file_path);
    print_last_write_time(file_path);

    std::cout << "\n=== Permissions ===\n";
    std::cout << "  " << permissions_to_string(fs::status(file_path).permissions()) << "\n";
    fs::permissions(file_path, fs::perms::owner_read, fs::perm_options::replace);
    std::cout << "  after replace: " << permissions_to_string(fs::status(file_path).permissions()) << "\n";
    fs::permissions(file_path, fs::perms::owner_write, fs::perm_options::add);

    std::cout << "\n=== Directory ===\n";
    std::cout << "cwd=" << fs::current_path() << "\n"
              << "tmp=" << fs::temp_directory_path() << "\n";
    if (fs::exists(dir_path) && fs::is_directory(dir_path)) {
        std::cout << "\nListing /tmp (first 5):\n";
        int shown = 0;
        for (const auto& entry : fs::directory_iterator{dir_path}) {
            if (shown++ >= 5) break;
            std::cout << "  " << entry.path() << "\n";
        }
    }

    std::cout << "\n=== Path manipulation ===\n";
    fs::path modified = file_path;
    modified.replace_filename("new_test.txt");
    std::cout << "replace_filename: " << modified << "\n";
    modified.replace_extension(".log");
    std::cout << "replace_extension: " << modified << "\n";
    const fs::path messy{"/tmp/../tmp/test.txt"};
    std::cout << "lexically_normal: " << messy.lexically_normal() << "\n";
    std::cout << "weakly_canonical: " << fs::weakly_canonical(messy) << "\n";

    fs::path joined = fs::path{"/tmp"} / "sub" / "leaf.txt";
    std::cout << "operator/ join: " << joined << "\n";
    joined += "_suffix";
    std::cout << "operator+= append: " << joined << "\n";

    std::cout << "\n=== Copy / rename / remove ===\n";
    const fs::path copy_target{"/tmp/test_copy.txt"};
    fs::copy_file(file_path, copy_target, fs::copy_options::overwrite_existing);
    std::cout << "copied to " << copy_target << ", exists=" << fs::exists(copy_target) << "\n";
    const fs::path renamed_target{"/tmp/test_renamed.txt"};
    fs::rename(copy_target, renamed_target);
    std::cout << "renamed to " << renamed_target << ", exists=" << fs::exists(renamed_target) << "\n";
    fs::remove(renamed_target);
    std::cout << "removed, exists=" << fs::exists(renamed_target) << "\n";

    std::cout << "\n=== Directory tree operations ===\n";
    demonstrate_directory_tree("/tmp/fs_demo_root");

    std::cout << "\n=== Disk space ===\n";
    {
        std::error_code ec;
        auto space = fs::space(dir_path, ec);
        if (!ec)
            std::cout << "capacity=" << space.capacity / (1024 * 1024) << " MiB, "
                      << "free=" << space.free / (1024 * 1024) << " MiB\n";
    }

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
    std::vector<fs::path> paths{file_path, modified, messy, joined};
    std::ranges::sort(paths);
    for (const auto& p : paths) std::cout << "  " << p << "\n";

    std::cout << "\n=== Cleanup ===\n";
    std::error_code ec;
    auto removed = fs::remove_all("/tmp/fs_demo_root", ec);
    std::cout << "removed " << removed << " entries under fs_demo_root\n";
    fs::remove(file_path, ec);
    std::cout << "removed test.txt: " << !ec << "\n";

    return 0;
}
