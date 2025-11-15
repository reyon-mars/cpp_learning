#include <iostream>
#include <vector>
#include <string>

int main ( void ){
    std::vector<char> v;

    std::string str_v( v.begin(), v.end() );
    std::string str;


    for( auto c : v )
        str += c;
        
    std::cout << str;
    
    return 0;
} 
