// ----------- MORE ADVANCED ADDITIONS -----------

#include <iostream>   // ✅ ADDED
#include <cfloat>     // for float constants
#include <limits>     // ✅ ADDED
#include <bitset>     // ✅ NEW
#include <cassert>    // ✅ NEW
#include <climits>    // ✅ NEW

// ----------------------------------------------

// ✅ NEW: print binary representation
template<typename T>
void print_bits(T value) {
    std::bitset<sizeof(T) * 8> bits(value);

    std::cout << "Bits: "
              << bits << "\n";
}

// ✅ NEW: swap demo
void swap_values(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

// ✅ NEW: reference demo
void reference_demo() {

    int x = 10;
    int& ref = x;

    ref = 20;

    std::cout << "x after reference modification: "
              << x << "\n";
}

// ✅ NEW: overflow demonstration
void overflow_demo() {

    unsigned char c = 255;

    std::cout << "Before overflow: "
              << static_cast<int>(c) << "\n";

    c++;

    std::cout << "After overflow: "
              << static_cast<int>(c) << "\n";
}

// ✅ NEW: type sizes
void print_type_sizes() {

    std::cout << "\nType Sizes:\n";

    std::cout << "sizeof(char): "
              << sizeof(char) << "\n";

    std::cout << "sizeof(int): "
              << sizeof(int) << "\n";

    std::cout << "sizeof(double): "
              << sizeof(double) << "\n";

    std::cout << "sizeof(long long): "
              << sizeof(long long) << "\n";
}

// ✅ NEW: ASCII demo
void ascii_demo() {

    char ch = 'A';

    std::cout << "\nASCII Demo:\n";

    std::cout << "Character: "
              << ch << "\n";

    std::cout << "ASCII value: "
              << static_cast<int>(ch) << "\n";
}

// ----------------------------------------------

int main() {

    std::cout << "\nAdvanced Fundamental Concepts:\n";

    // ✅ Signed vs unsigned behavior
    int neg = -1;
    unsigned int u_neg = neg;

    std::cout << "Signed -1 as unsigned: "
              << u_neg << "\n";

    // ✅ Floating point precision issue
    double x = 0.1 + 0.2;

    std::cout << "0.1 + 0.2 = "
              << x << "\n";

    // ✅ numeric_limits extra properties
    std::cout << "int is signed? "
              << std::numeric_limits<int>::is_signed << "\n";

    std::cout << "float epsilon: "
              << std::numeric_limits<float>::epsilon() << "\n";

    // ✅ Pointer basics
    int value = 42;
    int* ptr = &value;

    std::cout << "Pointer value: "
              << ptr << "\n";

    std::cout << "Dereferenced: "
              << *ptr << "\n";

    // nullptr usage
    int* null_ptr = nullptr;

    if (null_ptr == nullptr) {
        std::cout << "Pointer is null\n";
    }

    // ✅ Literal suffixes
    auto big_num = 10000000000LL;
    auto precise = 3.14L;

    std::cout << "big_num (long long): "
              << big_num << "\n";

    std::cout << "precise (long double): "
              << precise << "\n";

    // --------------------------------------------------
    // ✅ NEW FEATURE USAGE
    // --------------------------------------------------

    print_type_sizes();

    // Binary representation
    std::cout << "\nBinary of value 42:\n";
    print_bits(value);

    // Reference demo
    reference_demo();

    // Overflow demo
    overflow_demo();

    // ASCII demo
    ascii_demo();

    // Swap demo
    int a = 5;
    int b = 10;

    std::cout << "\nBefore swap: "
              << a << ", " << b << "\n";

    swap_values(a, b);

    std::cout << "After swap: "
              << a << ", " << b << "\n";

    // Numeric limits
    std::cout << "\nInteger Limits:\n";

    std::cout << "INT_MAX: "
              << INT_MAX << "\n";

    std::cout << "INT_MIN: "
              << INT_MIN << "\n";

    std::cout << "DBL_MAX: "
              << DBL_MAX << "\n";

    // Assertions
    assert(sizeof(int) >= 4);

    // Boolean fundamentals
    bool flag = true;

    std::cout << "\nBoolean value: "
              << flag << "\n";

    std::cout << "sizeof(bool): "
              << sizeof(bool) << "\n";

    // Character arithmetic
    char letter = 'A';

    std::cout << "\nNext character after A: "
              << static_cast<char>(letter + 1)
              << "\n";

    // --------------------------------------------------

    return 0;
}
