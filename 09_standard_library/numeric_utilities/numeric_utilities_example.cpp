// Numeric Utilities Exercise
// std::complex, std::ratio, std::gcd, std::lcm

#include <iostream>
#include <complex>
#include <numeric>

int main() {
    // Complex numbers
    std::complex<double> z1(3.0, 4.0);
    std::complex<double> z2(1.0, 2.0);
    
    std::cout << "z1: " << z1 << "\n";
    std::cout << "z1 + z2: " << (z1 + z2) << "\n";
    std::cout << "z1 * z2: " << (z1 * z2) << "\n";
    
    // GCD and LCM
    int a = 48, b = 18;
    std::cout << "GCD(48, 18): " << std::gcd(a, b) << "\n";
    std::cout << "LCM(48, 18): " << std::lcm(a, b) << "\n";
    
    return 0;
}
