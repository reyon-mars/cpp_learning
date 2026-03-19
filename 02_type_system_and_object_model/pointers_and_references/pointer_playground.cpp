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

// ----------- NEW ADDITIONS -----------

// Helper to print array
void print_array(int* arr, int n) {
    for(int i = 0; i < n; i++)
        std::cout << arr[i] << " ";
    std::cout << "\n";
}

// Reverse using indexing (comparison method)
void reverse_index(int* arr, int n) {
    for(int i = 0; i < n/2; i++) {
        int temp = arr[i];
        arr[i] = arr[n - i - 1];
        arr[n - i - 1] = temp;
    }
}

// Check if array is palindrome
bool is_palindrome(int* arr, int n) {
    for(int i = 0; i < n/2; i++) {
        if(arr[i] != arr[n - i - 1])
            return false;
    }
    return true;
}

// Copy array
int* copy_array(int* arr, int n) {
    int* newArr = new int[n];
    for(int i = 0; i < n; i++)
        newArr[i] = arr[i];
    return newArr;
}

// ------------------------------------

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


    // -------- NEW FEATURE USAGE --------

    // Copy array
    int* copy = copy_array(arr, n);
    std::cout << "Copied array: ";
    print_array(copy, n);

    // Reverse again using index method
    reverse_index(copy, n);
    std::cout << "Reversed copy (index method): ";
    print_array(copy, n);

    // Palindrome check
    std::cout << "Is original array palindrome? "
              << (is_palindrome(arr, n) ? "Yes\n" : "No\n");

    delete []copy;

    // ----------------------------------

    delete []arr;

    return 0;
}
