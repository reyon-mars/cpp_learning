#include <iostream>

int natural_sum( int n )
{
	int sum = 0;
	while( n )
	{
		sum += n;
		n--;
	}
	return sum;
}

int main ( void ){
    int sum = 0;
    int num = 50;
    while( num <= 100 ){
        sum += num;
        num++;
    }
    std::cout << sum  << std::endl;
    return 0;
}

// --------------------------------------------
// EXTRA ADDED CODE (original left untouched)
// --------------------------------------------

int sum_even(int n) {
    int total = 0;
    for(int i = 2; i <= n; i += 2)
        total += i;
    return total;
}

void print_series(int n) {
    std::cout << "Series from 1 to " << n << ": ";
    for(int i = 1; i <= n; i++)
        std::cout << i << " ";
    std::cout << std::endl;
}

int __ = [](){
    std::cout << "Extra: natural_sum(10) = " << natural_sum(10) << std::endl;
    std::cout << "Extra: sum_even(20) = " << sum_even(20) << std::endl;
    print_series(10);
    return 0;
}();

// ----------------------------------------------------
// ADDITIONAL EXTRA CODE (still NOT touching original)
// ----------------------------------------------------

int factorial(int n) {
    int f = 1;
    for(int i = 1; i <= n; i++)
        f *= i;
    return f;
}

void print_even_series(int n) {
    std::cout << "Even numbers up to " << n << ": ";
    for(int i = 2; i <= n; i += 2)
        std::cout << i << " ";
    std::cout << "\n";
}

// auto-run extra test
int ___ = [](){
    std::cout << "\nMore Extra:\n";
    std::cout << "Factorial(6) = " << factorial(6) << std::endl;
    print_even_series(20);
    return 0;
}();
