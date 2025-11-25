#include <iostream>
#include <memory>

struct User{
    std::string name;
    int age;
};

int main ( void ){
    auto u = std::make_unique<User>( "Mars", 22 );

    std::cout << u->name << " " << u->age << '\n';

    std::unique_ptr<User> v;
    v = std::move( u );

    if( u == nullptr ){
        std::cout << "U is nullptr ";
    }
    return 0;
}