#include <iostream>
#include <string>

std::string replace ( std::string& s, const std::string& old_val, const std::string& new_val ){
    auto pos = s.find( old_val );
    if( pos != std::string::npos) {
        s.replace( pos, old_val.size() , new_val );
    }
    return s;
}



int main() {
    std::string text = "Hello Mars";
    replace(text, "Mars", "World");
    std::cout << text << '\n';  // Output: Hello World
}