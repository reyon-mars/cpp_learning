#include <iostream>

// --------------------------------------------
// ORIGINAL EXTRA FUNCTIONS
// --------------------------------------------

// Sum of first n natural numbers
int natural_sum(int n) {
    return n * (n + 1) / 2;
}

// Sum of even numbers up to n
int sum_even(int n) {
    int total = 0;
    for (int i = 2; i <= n; i += 2)
        total += i;
    return total;
}

// Sum of odd numbers up to n
int sum_odd(int n) {
    int total = 0;
    for (int i = 1; i <= n; i += 2)
        total += i;
    return total;
}

// Factorial of n
int factorial(int n) {
    int f = 1;
    for (int i = 1; i <= n; i++)
        f *= i;
    return f;
}

// Print series from 1 to n
void print_series(int n) {
    std::cout << "Series from 1 to " << n << ": ";
    for (int i = 1; i <= n; i++)
        std::cout << i << " ";
    std::cout << std::endl;
}

// Print even numbers up to n
void print_even_series(int n) {
    std::cout << "Even numbers up to " << n << ": ";
    for (int i = 2; i <= n; i += 2)
        std::cout << i << " ";
    std::cout << std::endl;
}

// --------------------------------------------
// AUTO-EXECUTED EXTRA DEMOS (RUNS BEFORE MAIN)
// --------------------------------------------

int __extra_demo = [](){
    std::cout << "\n--- Extra Examples (Auto-run before main) ---\n";

    std::cout << "natural_sum(10) = " << natural_sum(10) << std::endl;
    std::cout << "sum_even(20) = " << sum_even(20) << std::endl;
    std::cout << "sum_odd(15) = " << sum_odd(15) << std::endl;
    std::cout << "factorial(6) = " << factorial(6) << std::endl;

    print_series(10);
    print_even_series(20);

    return 0;
}();

// --------------------------------------------
// SINGLE ENTRY FUNCTION (CALLABLE FROM MAIN)
// --------------------------------------------

void run_extra_examples() {
    std::cout << "\n--- Extra Examples (Callable from main) ---\n";

    std::cout << "natural_sum(10) = " << natural_sum(10) << std::endl;
    std::cout << "sum_even(20) = " << sum_even(20) << std::endl;
    std::cout << "sum_odd(15) = " << sum_odd(15) << std::endl;
    std::cout << "factorial(6) = " << factorial(6) << std::endl;

    print_series(10);
    print_even_series(20);
}

// --------------------------------------------
// MAIN
// --------------------------------------------

int main() {
    std::cout << "\n=== Main Function Started ===\n";

    // Optional: call extra examples again from main
    run_extra_examples();

    std::cout << "\n=== Program Finished ===\n";
    return 0;
}
 
