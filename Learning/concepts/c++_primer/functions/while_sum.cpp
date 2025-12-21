// --------------------------------------------
// EXTRA ADDED CODE (append only, no auto-run)
// --------------------------------------------

int sum_even(int n) {
    int total = 0;
    for (int i = 2; i <= n; i += 2)
        total += i;
    return total;
}

int sum_odd(int n) {
    int total = 0;
    for (int i = 1; i <= n; i += 2)
        total += i;
    return total;
}

int factorial(int n) {
    int f = 1;
    for (int i = 1; i <= n; i++)
        f *= i;
    return f;
}

void print_series(int n) {
    std::cout << "Series from 1 to " << n << ": ";
    for (int i = 1; i <= n; i++)
        std::cout << i << " ";
    std::cout << std::endl;
}

void print_even_series(int n) {
    std::cout << "Even numbers up to " << n << ": ";
    for (int i = 2; i <= n; i += 2)
        std::cout << i << " ";
    std::cout << std::endl;
}

// --------------------------------------------
// SINGLE ENTRY FUNCTION FOR EXTRAS
// --------------------------------------------

void run_extra_examples() {
    std::cout << "\n--- Extra Examples ---\n";

    std::cout << "natural_sum(10) = "
              << natural_sum(10) << std::endl;

    std::cout << "sum_even(20) = "
              << sum_even(20) << std::endl;

    std::cout << "sum_odd(15) = "
              << sum_odd(15) << std::endl;

    std::cout << "factorial(6) = "
              << factorial(6) << std::endl;

    print_series(10);
    print_even_series(20);
}
