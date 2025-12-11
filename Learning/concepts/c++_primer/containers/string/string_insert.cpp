#include <iostream>
#include <string>

std::string replace_idx( std::string& s, const std::string& old_val, const std::string& new_val ){
    auto pos = s.find( old_val );
    if( pos != std::string::npos) {
        s.replace( pos, old_val.size() , new_val );
    }
    return s;
}

int main() {
    std::string prefix = "mr";
    std::string suffix = "smith";

    std::string concat = prefix + suffix;

    std::cout << concat << std::endl;

    std::string text = "Hello Mars";
    replace_idx(text, "Mars", "World");
    std::cout << text << '\n';  // Output: Hello World
}


// ======================================================
// EXTRA CODE ADDED BELOW (original code unchanged)
// ======================================================

std::string replace_all(std::string s, const std::string& old_val, const std::string& new_val) {
    size_t pos = 0;
    while ((pos = s.find(old_val, pos)) != std::string::npos) {
        s.replace(pos, old_val.size(), new_val);
        pos += new_val.size();
    }
    return s;
}

void printInfo() {
    std::cout << "[Extra] Replace utility running!" << std::endl;
}

int runExtra = [](){
    printInfo();
    std::string test = "foo bar foo bar foo";
    std::cout << "[Extra] Before: " << test << '\n';
    test = replace_all(test, "foo", "baz");
    std::cout << "[Extra] After : " << test << '\n';
    return 0;
}();
