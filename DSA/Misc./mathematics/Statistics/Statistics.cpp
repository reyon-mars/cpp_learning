#include <vector>
#include <iostream>
#include <limits>

int main( void ){

    int N(0), input(0);
    std::vector<int> numbers;

    std::cout << "Please enter the size of Vector : ";
    std::cin >> N;
    numbers.reserve(N);

    while( N-- ){
        std::cout << "\nPlease enter a number: ";
        std::cin >> input;
        numbers.push_back(input);
        std::cout << "\n";
    }

    int sum(0);
    int maxVal = std::numeric_limits<int>::min();
    int minVal = std::numeric_limits<int>::max();

    for( auto number: numbers ){
        if( number > maxVal ){
            maxVal = number;
        }
        if( number < minVal ){
            minVal = number;
        }
        sum += number;
    }
    double Mean = static_cast<int> (sum)/numbers.size();
    std::cout << "Mean: " << Mean << "\n";
    std::cout << "Max: " << maxVal << "\n";
    std::cout << "Min: " << minVal << "\n";

    return 0;
}