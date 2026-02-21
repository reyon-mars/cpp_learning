#include <iostream>
#include <chrono>
#include <string>


class scoped_timer {
    
    private:
    std::chrono::steady_clock::time_point start;
    std::string name;
    
    public:
    explicit scoped_timer( std::string str ) : start(std::chrono::steady_clock::now()), name( std::move(str )){}

    ~scoped_timer(){
        auto end = std::chrono::steady_clock::now();
        std::cout << name << ':' << std::chrono::duration_cast<std::chrono::microseconds> ( end - start ).count() << " us" << '\n';
    }
};