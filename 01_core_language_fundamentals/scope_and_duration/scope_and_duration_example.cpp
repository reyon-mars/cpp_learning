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

// ----------- NEW ADDITIONS -----------

// Function showing automatic variable lifecycle
void automatic_example() {
    int temp = 5; // created every call
    std::cout << "automatic_example temp: " << temp << '\n';
}

// Function showing dynamic allocation inside function
void dynamic_example() {
    int* ptr = new int(100);
    std::cout << "dynamic_example value: " << *ptr << '\n';
    delete ptr;
}

// Static variable behaving like global but scoped to function
void static_demo() {
    static int value = 50;
    value += 10;
    std::cout << "static_demo value: " << value << '\n';
}

// ------------------------------------

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

    // -------- NEW FEATURE USAGE --------

    automatic_example();
    automatic_example(); // recreated each call

    dynamic_example();

    static_demo();
    static_demo(); // persists like global

    // Safety check
    if (heap_var != nullptr) {
        std::cout << "Heap pointer is valid\n";
    }

    // ----------------------------------

    delete heap_var;   // free dynamic memory
    heap_var = nullptr;

    return 0;
}
