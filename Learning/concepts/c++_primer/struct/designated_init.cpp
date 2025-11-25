#include  <iostream>
#include <string>



struct Book{
    std::string name;
    int ISBN_NO;
    std::string publisher;
};

int main ( void ){
    Book b1 = {
        .name = "test",
        .ISBN_NO = 12,
        .publisher = "O'Rilley"
    };
    
}