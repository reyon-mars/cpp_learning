// ----------- MORE ADVANCED ADDITIONS -----------

#include <iostream>   // ✅ ADDED
#include <cfloat>     // for float constants
#include <limits>     // ✅ ADDED

// ----------------------------------------------

int main() {

    std::cout << "\nAdvanced Fundamental Concepts:\n";

    // ✅ Signed vs unsigned behavior
    int neg = -1;
    unsigned int u_neg = neg;
    std::cout << "Signed -1 as unsigned: " << u_neg << "\n";

    // ✅ Floating point precision issue
    double x = 0.1 + 0.2;
    std::cout << "0.1 + 0.2 = " << x << "\n";

    // ✅ numeric_limits extra properties
    std::cout << "int is signed? "
              << std::numeric_limits<int>::is_signed << "\n";

    std::cout << "float epsilon: "
              << std::numeric_limits<float>::epsilon() << "\n";

    // ✅ Pointer basics
    int value = 42;
    int* ptr = &value;

    std::cout << "Pointer value: " << ptr << "\n";
    std::cout << "Dereferenced: " << *ptr << "\n";

    // nullptr usage
    int* null_ptr = nullptr;
    if (null_ptr == nullptr) {
        std::cout << "Pointer is null\n";
    }

    // ✅ Literal suffixes
    auto big_num = 10000000000LL;
    auto precise = 3.14L;

    std::cout << "big_num (long long): " << big_num << "\n";
    std::cout << "precise (long double): " << precise << "\n";

    return 0;
}
