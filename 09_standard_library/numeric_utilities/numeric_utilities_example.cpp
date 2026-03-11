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

    return 0;
}
