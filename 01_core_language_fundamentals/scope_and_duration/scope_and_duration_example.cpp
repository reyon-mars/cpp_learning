// Scope and Duration Exercise
// Automatic, static, dynamic storage duration

#include <iostream>

int global_var = 0;  // Static storage duration

int main() {
    int local_var = 10;  // Automatic storage duration
    static int static_var = 20;  // Static storage duration
    int* heap_var = new int(30);  // Dynamic storage duration
    
    delete heap_var;
    return 0;
}
