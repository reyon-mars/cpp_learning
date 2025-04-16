#include <iostream>
#include <vector>
#include <unordered_map>
#include <climits>

int main(){

    int N(0);
    std::cin >> N;
    
    std::vector<int> array(N);
    for( int i = 0; i < N; i++ ){
        std::cin >> array[i];
    }

    int minIndex( INT_MAX );
    std::unordered_map< int, int > seen;
    
    for( int index = 0; index < array.size(); index++ ){
        int number = array[index];

        if( seen.find( number ) == seen.end() ){
            seen[number] = index;
        } else {
            if( seen[number] < minIndex ){
                minIndex = seen[number];
            }
        }
    }
    
    if( minIndex != INT_MAX ){
                std::cout << minIndex;
                std::cout << "The first repeating number in the array is "; 
                std::cout << array[minIndex] << std::endl;
        } else {
            std::cout << "There is no repeating element in the array." << std::endl;
        }
}