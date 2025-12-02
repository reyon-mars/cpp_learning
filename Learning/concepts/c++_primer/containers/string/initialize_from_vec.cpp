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

    // ---------- MORE ADDED CODE BELOW ----------
    
    // print size of vector
    std::cout << "Vector size: " << v.size() << std::endl;

    // append another character
    v.push_back('~');
    std::cout << "After adding '~', vector size: " << v.size() << std::endl;

    // print updated vector as string
    std::string updated(v.begin(), v.end());
    std::cout << "Updated string: " << updated << std::endl;

    // reverse print
    std::cout << "Reverse: ";
    for(auto it = v.rbegin(); it != v.rend(); ++it)
        std::cout << *it;
    std::cout << std::endl;

    // ------------------------------------------

    return 0;
}
