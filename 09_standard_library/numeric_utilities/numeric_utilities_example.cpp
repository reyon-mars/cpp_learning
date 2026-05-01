// Numeric Utilities Exercise
// std::complex, std::ratio, std::gcd, std::lcm

#include <iostream>
#include <complex>
#include <numeric>
#include <ratio>

int main() {
    // Complex numbers
    std::complex<double> z1(3.0, 4.0);
    std::complex<double> z2(1.0, 2.0);
    
    std::cout << "z1: " << z1 << "\n";
    std::cout << "z1 + z2: " << (z1 + z2) << "\n";
    std::cout << "z1 * z2: " << (z1 * z2) << "\n";

    // ---- Additional complex utilities ----
    std::cout << "Magnitude of z1: " << std::abs(z1) << "\n";
    std::cout << "Phase of z1: " << std::arg(z1) << "\n";
    // --------------------------------------

    // GCD and LCM
    int a = 48, b = 18;
    std::cout << "GCD(48, 18): " << std::gcd(a, b) << "\n";
    std::cout << "LCM(48, 18): " << std::lcm(a, b) << "\n";

    // ---- std::ratio example ----
    using half = std::ratio<1, 2>;
    using quarter = std::ratio<1, 4>;

    std::cout << "Half ratio: " << half::num << "/" << half::den << "\n";
    std::cout << "Quarter ratio: " << quarter::num << "/" << quarter::den << "\n";

    // ratio addition
    using sum_ratio = std::ratio_add<half, quarter>;
    std::cout << "Half + Quarter = "
              << sum_ratio::num << "/" << sum_ratio::den << "\n";
    // --------------------------------

    // --------------------------------
    // EXTRA SMALL ADDITIONS (NEW)
    // --------------------------------

    // Complex conjugate
    std::cout << "Conjugate of z1: " << std::conj(z1) << "\n";

    // Real and imaginary parts
    std::cout << "Real part of z1: " << z1.real() << "\n";
    std::cout << "Imaginary part of z1: " << z1.imag() << "\n";

    // Power of complex number
    std::cout << "z1 squared: " << std::pow(z1, 2) << "\n";

    // Additional GCD/LCM example
    int x = 21, y = 6;
    std::cout << "GCD(21, 6): " << std::gcd(x, y) << "\n";
    std::cout << "LCM(21, 6): " << std::lcm(x, y) << "\n";

    // ratio multiplication
    using mul_ratio = std::ratio_multiply<half, quarter>;
    std::cout << "Half * Quarter = "
              << mul_ratio::num << "/" << mul_ratio::den << "\n";

    // ratio simplification example
    using simplified = std::ratio<2, 4>;
    std::cout << "Simplified ratio of 2/4 = "
              << simplified::num << "/" << simplified::den << "\n";

    // --------------------------------

    return 0;
}
