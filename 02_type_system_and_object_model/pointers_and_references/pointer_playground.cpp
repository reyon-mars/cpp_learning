#include <iostream>
#include <cstdlib>

// ----------- NEW ADDITIONS -----------

// Find max element
int find_max(int* arr, int n) {
    int max = arr[0];
    for(int i = 1; i < n; i++)
        if(arr[i] > max)
            max = arr[i];
    return max;
}

// Find min element
int find_min(int* arr, int n) {
    int min = arr[0];
    for(int i = 1; i < n; i++)
        if(arr[i] < min)
            min = arr[i];
    return min;
}

// Sum of array
int sum_array(int* arr, int n) {
    int sum = 0;
    for(int i = 0; i < n; i++)
        sum += arr[i];
    return sum;
}

// Average
double average_array(int* arr, int n) {
    return static_cast<double>(sum_array(arr, n)) / n;
}

// Count even numbers
int count_even(int* arr, int n) {
    int count = 0;
    for(int i = 0; i < n; i++)
        if(arr[i] % 2 == 0)
            count++;
    return count;
}

// Count odd numbers
int count_odd(int* arr, int n) {
    int count = 0;
    for(int i = 0; i < n; i++)
        if(arr[i] % 2 != 0)
            count++;
    return count;
}

// Linear search
int search_element(int* arr, int n, int key) {
    for(int i = 0; i < n; i++)
        if(arr[i] == key)
            return i;
    return -1;
}

// Shift array left
void shift_left(int* arr, int n) {
    if(n <= 1) return;
    int first = arr[0];
    for(int i = 0; i < n - 1; i++)
        arr[i] = arr[i + 1];
    arr[n - 1] = first;
}

// ------------------------------------


// ================= YOUR ORIGINAL CODE =================

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

void print_array(int* arr, int n) {
    for(int i = 0; i < n; i++)
        std::cout << arr[i] << " ";
    std::cout << "\n";
}

void reverse_index(int* arr, int n) {
    for(int i = 0; i < n/2; i++) {
        int temp = arr[i];
        arr[i] = arr[n - i - 1];
        arr[n - i - 1] = temp;
    }
}

bool is_palindrome(int* arr, int n) {
    for(int i = 0; i < n/2; i++) {
        if(arr[i] != arr[n - i - 1])
            return false;
    }
    return true;
}

int* copy_array(int* arr, int n) {
    int* newArr = new int[n];
    for(int i = 0; i < n; i++)
        newArr[i] = arr[i];
    return newArr;
}

int main(){
    int value(0);
    int *ptr = &value;

    std::cout << ptr << "\n" << *ptr << "\n";

    int n(0);
    std::cout << "Please enter the size of the array: ";
    std::cin >> n;

    if(n <= 0){
        std::cout << "Invalid array size\n";
        return 0;
    }

    int *arr = new int[n];
    for( int i = 0; i < n; i++ ){
        arr[i] = i;
    }

    std::cout << "Before reverse: ";
    for(int i = 0; i < n; i++){
        std::cout << arr[i] << " ";
    }
    std::cout << "\n";

    reverse_array( arr, n );

    std::cout << "After reverse:  ";
    for(int i = 0; i < n; i++){
        std::cout << arr[i] << " ";
    }
    std::cout << "\n";

    // -------- NEW FEATURE USAGE --------

    int* copy = copy_array(arr, n);
    std::cout << "Copied array: ";
    print_array(copy, n);

    reverse_index(copy, n);
    std::cout << "Reversed copy (index method): ";
    print_array(copy, n);

    std::cout << "Is original array palindrome? "
              << (is_palindrome(arr, n) ? "Yes\n" : "No\n");

    // -------- ADDITION USAGE --------

    std::cout << "\nMax element: " << find_max(arr, n) << "\n";
    std::cout << "Min element: " << find_min(arr, n) << "\n";

    std::cout << "Sum: " << sum_array(arr, n) << "\n";
    std::cout << "Average: " << average_array(arr, n) << "\n";

    std::cout << "Even count: " << count_even(arr, n) << "\n";
    std::cout << "Odd count: " << count_odd(arr, n) << "\n";

    int key = 3;
    int pos = search_element(arr, n, key);
    if(pos != -1)
        std::cout << "Element " << key << " found at index " << pos << "\n";
    else
        std::cout << "Element not found\n";

    shift_left(arr, n);
    std::cout << "After left shift: ";
    print_array(arr, n);

    // ----------------------------------

    delete []copy;
    delete []arr;

    return 0;
}
