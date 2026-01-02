#include <iostream>

// --------------------------------------------
// ORIGINAL FUNCTIONS (UNCHANGED LOGIC)
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
// SMALL ADDITIONAL FUNCTION
// --------------------------------------------

// Display all results together
void show_results(int n) {
    std::cout << "\nResults for n = " << n << std::endl;
    std::cout << "Natural sum: " << natural_sum(n) << std::endl;
    std::cout << "Even sum: " << sum_even(n) << std::endl;
    std::cout << "Odd sum: " << sum_odd(n) << std::endl;
    std::cout << "Factorial: " << factorial(n) << std::endl;
    print_series(n);
    print_even_series(n);
}

// --------------------------------------------
// MAIN
// --------------------------------------------

int main() {

    std::cout << "=== Program Started ===" << std::endl;

    show_results(10);   // small extra usage

    std::cout << "\n=== Program Finished ===" << std::endl;
    return 0;
}
