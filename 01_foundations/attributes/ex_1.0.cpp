#include <iostream>


// [[nodiscard]]
//
// The compiler issues a warning if the caller
// ignore the return value.
[[nodiscard]] int Area( int width, int height )
{
    return width * height;
}


// [[deprecated]]
// The compiler emmits a warning that this function
// has been depricated with the given message.
[[deprecated("This function has been depricated")]]
void test()
{
    std::cout << "Invoked test" << '\n';
}