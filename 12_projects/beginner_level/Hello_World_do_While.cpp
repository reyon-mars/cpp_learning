#include <iostream>

int main ( void ){
    int n(0), cnt(1);
    std::cout << "Please enter a number : ";
    std::cin >> n ;
    constexpr const char* str = "Hello World !";

    do{
        std::cout << str << "\n";
        cnt++;
    }while (cnt <= n);

    return 0;
}