// Fundamental Types Exercise
// Integral, floating-point, and other fundamental types

#include <iostream>
#include <limits>

int main() {
    // Integral types
    char c = 'A';
    short s = 100;
    int i = 1000;
    long l = 100000;
    long long ll = 1000000000000LL;

    // Floating-point types
    float f = 3.14f;
    double d = 3.141592653589793;
    long double ld = 3.141592653589793L;

    // Boolean
    bool b = true;

    // ---- Added output for clarity ----
    std::cout << "char: " << c << "\n";
    std::cout << "short: " << s << "\n";
    std::cout << "int: " << i << "\n";
    std::cout << "long: " << l << "\n";
    std::cout << "long long: " << ll << "\n";

    std::cout << "float: " << f << "\n";
    std::cout << "double: " << d << "\n";
    std::cout << "long double: " << ld << "\n";

    std::cout << "bool: " << std::boolalpha << b << "\n";
    // ----------------------------------

    // Display limits
    std::cout << "int max: " << std::numeric_limits<int>::max() << "\n";
    std::cout << "int min: " << std::numeric_limits<int>::min() << "\n";
    std::cout << "double max: " << std::numeric_limits<double>::max() << "\n";
    std::cout << "double min: " << std::numeric_limits<double>::min() << "\n";


    // -------- NEW FEATURE USAGE --------

    // Size of each type
    std::cout << "\nSizes (in bytes):\n";
    std::cout << "char: " << sizeof(c) << "\n";
    std::cout << "int: " << sizeof(i) << "\n";
    std::cout << "double: " << sizeof(d) << "\n";
    std::cout << "long long: " << sizeof(ll) << "\n";

    // Unsigned types
    unsigned int ui = 4000000000U;
    std::cout << "unsigned int: " << ui << "\n";

    // More limits
    std::cout << "char max: " << (int)std::numeric_limits<char>::max() << "\n";
    std::cout << "float max: " << std::numeric_limits<float>::max() << "\n";

    // Type casting
    int int_val = 10;
    double converted = static_cast<double>(int_val);

    std::cout << "Converted int to double: " << converted << "\n";

    // ASCII value
    std::cout << "ASCII value of 'A': " << static_cast<int>(c) << "\n";

    // Boolean as integer
    std::cout << "bool as int: " << static_cast<int>(b) << "\n";

    // ----------------------------------

    return 0;
}
