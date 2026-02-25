#include <iostream>

int* reverse_array( int *array, int n ){
    int start(0), end( n-1 );
    while( start < end ){
        int temp = *(array+start);
        *(array+start) = *(array+end);
        *(array+end) = temp;
        start++;
        end--;
    }
    return array;
}

int main(){
    int value(0);
    int *ptr = &value;

    std::cout << ptr << "\n" << *ptr << "\n";

    int n(0);
    std::cout << "Please enter the size of the array: ";
    std::cin >> n;

    // ✅ added size check
    if(n <= 0){
        std::cout << "Invalid array size\n";
        return 0;
    }

    int *arr = new int[n];
    for( int i = 0; i < n; i++ ){
        arr[i] = i;
    }

    // ✅ added print before reverse
    std::cout << "Before reverse: ";
    for(int i = 0; i < n; i++){
        std::cout << arr[i] << " ";
    }
    std::cout << "\n";

    reverse_array( arr, n );

    // ✅ added print after reverse
    std::cout << "After reverse:  ";
    for(int i = 0; i < n; i++){
        std::cout << arr[i] << " ";
    }
    std::cout << "\n";

    delete []arr;

    return 0;
}
