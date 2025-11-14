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