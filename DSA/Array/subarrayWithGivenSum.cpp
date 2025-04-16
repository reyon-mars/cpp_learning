#include <iostream>
#include <vector>

std::pair<int, int>  findSubarray( std::vector<int>& arr , int targetSum ){
    std::vector<int>::iterator front, back;
    front = back = arr.begin();
    int currentSum(0);

    while( targetSum != currentSum && front != arr.end() ){
        
        if( currentSum < targetSum ){
            currentSum += *front;
            front++;

        } else if ( targetSum < currentSum ){
            currentSum -= *back;
            back++;

        }
    }
    if( targetSum != currentSum ){ return }
    return { back - arr.begin(), front - arr.begin() };
}

int main(){

}