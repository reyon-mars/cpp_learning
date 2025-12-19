// --------------------------------------------
// EXTRA ADDED CODE (append only)
// --------------------------------------------

int sum_even(int n) {
    int total = 0;
    for (int i = 2; i <= n; i += 2)
        total += i;
    return total;
}

void print_series(int n) {
    std::cout << "Series from 1 to " << n << ": ";
    for (int i = 1; i <= n; i++)
        std::cout << i << " ";
    std::cout << std::endl;
}

// Runs automatically before main()
int __ = [](){
    std::cout << "Extra: natural_sum(10) = "
              << natural_sum(10) << std::endl;
    std::cout << "Extra: sum_even(20) = "
              << sum_even(20) << std::endl;
    print_series(10);
    return 0;
}();


// --------------------------------------------
// ADDITIONAL EXTRA CODE
// --------------------------------------------

int factorial(int n) {
    int f = 1;
    for (int i = 1; i <= n; i++)
        f *= i;
    return f;
}

void print_even_series(int n) {
    std::cout << "Even numbers up to " << n << ": ";
    for (int i = 2; i <= n; i += 2)
        std::cout << i << " ";
    std::cout << std::endl;
}

int ___ = [](){
    std::cout << "\nMore Extra:\n";
    std::cout << "Factorial(6) = " << factorial(6) << std::endl;
    print_even_series(20);
    return 0;
}();


// --------------------------------------------
// FINAL EXTRA ADDITION
// --------------------------------------------

int sum_odd(int n) {
    int total = 0;
    for (int i = 1; i <= n; i += 2)
        total += i;
    return total;
}

int ____ = [](){
    std::cout << "\nFinal Extra:\n";
    std::cout << "Sum of odd numbers up to 15 = "
              << sum_odd(15) << std::endl;
    return 0;
}();
