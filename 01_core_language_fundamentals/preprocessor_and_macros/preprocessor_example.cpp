// Preprocessor and Macros Exercise
// #define, #ifdef, include guards

#ifndef MY_HEADER_H
#define MY_HEADER_H

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define STRINGIFY(x) #x
#define CONCAT(a, b) a ## b

#include <iostream>

int main() {
    std::cout << "Max of 5 and 10: " << MAX(5, 10) << "\n";
    std::cout << "Stringify: " << STRINGIFY(hello) << "\n";
    return 0;
}

#endif
