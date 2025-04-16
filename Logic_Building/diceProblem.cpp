#include <iostream>

int main(){

    int faceNumber(0);

    while( true ){
        std::cout << "Please enter the dice face : ";
        std::cin >> faceNumber;

        int oppositeFace = 6 - faceNumber + 1;

        std::cout << "The face opposite to " << faceNumber << " is " << oppositeFace << std::endl;
    }

    return 0;
}