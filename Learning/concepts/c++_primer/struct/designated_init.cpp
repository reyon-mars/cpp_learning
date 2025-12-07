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

    std::cout << "Book Details:\n";
    std::cout << "Name: " << b1.name << std::endl;
    std::cout << "ISBN: " << b1.ISBN_NO << std::endl;
    std::cout << "Publisher: " << b1.publisher << std::endl;

    Book b2;
    b2.name = "C++ Primer";
    b2.ISBN_NO = 999;
    b2.publisher = "Pearson";

    std::cout << "\nSecond Book:\n";
    std::cout << "Name: " << b2.name << std::endl;
    std::cout << "ISBN: " << b2.ISBN_NO << std::endl;
    std::cout << "Publisher: " << b2.publisher << std::endl;

}
