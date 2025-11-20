#include <string>
#include <iostream>

void log_msg( std::string_view tag, std::string_view msg )
{
    std::cout << '[' << tag << ']' << msg << '\n';
    return ;
}

void log_msg_( std::string tag , std::string msg )
{
    std::cout << '[' << tag << ']' << msg << '\n';
    return ;
}


int main ( void ){
    return 0;
}