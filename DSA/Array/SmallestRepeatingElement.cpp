#include <iostream>
#include <unordered_map>
#include <vector>
#include <climits>

int main(){

    int N(0);
    std::cout << "Please enter a number: ";
    std::cin >> N;

    std::vector <int> array;
    array.resize(N);

    for( int i = 0; i < N; i++ ){
        std::cin >> array[i];
    }

    int minDuplicate( INT_MAX );
    std::unordered_map <int, bool> seen;


    for ( const auto number: array ){

        if( seen.find(number) != seen.end() ){
            if( number < minDuplicate && number > 0 ){
                minDuplicate = number;
            }
        }
        seen[number] = true;

    }
    std::cout << minDuplicate << std::endl;

}