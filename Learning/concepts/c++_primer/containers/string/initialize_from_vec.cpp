#include <iostream>
#include <vector>
#include <string>

int main ( void ){
    std::vector<char> v;

    // --- added lines ---
    v.push_back('H');
    v.push_back('i');
    v.push_back('!');
    // --------------------

    std::string str_v( v.begin(), v.end() );
    std::string str;

    for( auto c : v )
        str += c;
        
    std::cout << str;

    // --- added lines ---
    std::cout << "\nString created using constructor: " << str_v << std::endl;
    // --------------------

    return 0;
}

