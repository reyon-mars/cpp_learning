// Scope and Duration Exercise
// Automatic, static, dynamic storage duration

#include <iostream>

int global_var = 0;  // Static storage duration (global)

// helper to show static local behavior
void static_counter() {
    static int count = 0;   // Static storage duration
    ++count;
    std::cout << "static_counter value: " << count << '\n';
}

int main() {
    int local_var = 10;          // Automatic storage duration
    static int static_var = 20;  // Static storage duration
    int* heap_var = new int(30); // Dynamic storage duration

    // ---- small added usage ----
    std::cout << "global_var: " << global_var << '\n';
    std::cout << "local_var: " << local_var << '\n';
    std::cout << "static_var: " << static_var << '\n';
    std::cout << "heap_var: " << *heap_var << '\n';

    static_counter();
    static_counter();  // shows persistence of static local variable
    // --------------------------

    delete heap_var;   // free dynamic memory
    heap_var = nullptr;

    return 0;
}
