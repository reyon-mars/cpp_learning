#include <iostream>

std::istream& readUntilEOF( std::istream& input){
    std::string data;
    while( input >> data ){
        std::cout << data;
        std::cout << " ";
    }
    input.clear();
    return input;
}

int main( void ){
    
}