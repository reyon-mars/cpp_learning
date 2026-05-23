// Numeric Utilities Exercise
// std::complex, std::ratio, std::gcd, std::lcm

#include <iostream>
#include <complex>
#include <numeric>
#include <ratio>
#include <vector>
#include <algorithm>

// ---------------- SMALL EXTRA HELPERS ----------------

// print complex number details
void print_complex_info(const std::complex<double>& z) {
    std::cout << "Value: " << z << "\n";
    std::cout << "Magnitude: " << std::abs(z) << "\n";
    std::cout << "Phase: " << std::arg(z) << "\n";
}

// check if number is coprime
bool are_coprime(int a, int b) {
    return std::gcd(a, b) == 1;
}

// ---------------- MAIN ----------------

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
    // ADDITIONAL SMALL CODE
    // --------------------------------

    // print helper demo
    std::cout << "\nComplex info for z2:\n";
    print_complex_info(z2);

    // polar form
    std::complex<double> polar_z = std::polar(5.0, 0.5);
    std::cout << "Polar complex number: " << polar_z << "\n";

    // norm of complex number
    std::cout << "Norm of z1: " << std::norm(z1) << "\n";

    // coprime check
    std::cout << "Are 35 and 64 coprime? "
              << (are_coprime(35, 64) ? "Yes" : "No") << "\n";

    // ratio subtraction
    using sub_ratio = std::ratio_subtract<half, quarter>;
    std::cout << "Half - Quarter = "
              << sub_ratio::num << "/" << sub_ratio::den << "\n";

    // ratio division
    using div_ratio = std::ratio_divide<half, quarter>;
    std::cout << "Half / Quarter = "
              << div_ratio::num << "/" << div_ratio::den << "\n";

    // gcd/lcm for vector values
    std::vector<int> nums = {12, 18, 24};

    int gcd_all = nums[0];
    int lcm_all = nums[0];

    for (size_t i = 1; i < nums.size(); ++i) {
        gcd_all = std::gcd(gcd_all, nums[i]);
        lcm_all = std::lcm(lcm_all, nums[i]);
    }

    std::cout << "GCD of vector values: " << gcd_all << "\n";
    std::cout << "LCM of vector values: " << lcm_all << "\n";

    // --------------------------------

    return 0;
}
