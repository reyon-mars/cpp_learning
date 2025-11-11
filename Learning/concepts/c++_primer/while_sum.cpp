#include <iostream>

int main ( void ){
    int sum = 0;
    int num = 50;
    while( num <= 100 ){
        sum += num;
        num++;
    }
    std::cout << sum  << std::endl;
    return 0;
}