#include <string>
#include "scoped_timer.hpp"
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


int main ( void )
{
    const int iterations = 1000000;
    std::string test_tag = "Tag";
    std::string test_msg = "Message";

    // Benchmark log_msg (string_view)
    {
        scoped_timer timer_sv("String View (Total)");
        for (int i = 0; i < iterations; ++i) {
            log_msg(test_tag, test_msg);
        }
    } // Timer automatically stops and reports here

    std::cout << "\n";

    // Benchmark log_msg_ (string copy)
    {
        scoped_timer timer_str("Normal String (Total)");
        for (int i = 0; i < iterations; ++i) {
            log_msg_(test_tag, test_msg);
        }
    } // Timer automatically stops and reports here
    
    return 0;
}