#include <iostream>
#include <complex>
#include <numeric>
#include <ratio>
#include <vector>
#include <string_view>

void print_complex(const std::complex<double>& z, std::string_view label = "") {
    if (!label.empty()) std::cout << label << ":\n";
    std::cout << "  value="  << z            << "\n"
              << "  real="   << z.real()      << "\n"
              << "  imag="   << z.imag()      << "\n"
              << "  abs="    << std::abs(z)   << "\n"
              << "  arg="    << std::arg(z)   << "\n"
              << "  norm="   << std::norm(z)  << "\n"
              << "  conj="   << std::conj(z)  << "\n";
}

template<typename R>
void print_ratio(std::string_view label) {
    std::cout << label << "=" << R::num << "/" << R::den << "\n";
}

[[nodiscard]] bool are_coprime(int a, int b) noexcept {
    return std::gcd(a, b) == 1;
}

int main() {
    std::cout << "=== Complex numbers ===\n";
    const std::complex<double> z1{3.0, 4.0};
    const std::complex<double> z2{1.0, 2.0};

    print_complex(z1, "z1");
    print_complex(z2, "z2");

    std::cout << "z1+z2="     << z1 + z2                << "\n"
              << "z1*z2="     << z1 * z2                << "\n"
              << "z1^2="      << std::pow(z1, 2)        << "\n"
              << "polar(5,0.5)=" << std::polar(5.0, 0.5) << "\n";

    std::cout << "\n=== GCD / LCM ===\n";
    std::cout << std::boolalpha
              << "gcd(48,18)="  << std::gcd(48, 18)   << "\n"
              << "lcm(48,18)="  << std::lcm(48, 18)   << "\n"
              << "gcd(21,6)="   << std::gcd(21,  6)   << "\n"
              << "lcm(21,6)="   << std::lcm(21,  6)   << "\n"
              << "coprime(35,64)=" << are_coprime(35, 64) << "\n";

    std::cout << "\n=== GCD / LCM over vector ===\n";
    const std::vector<int> nums{12, 18, 24};
    const int gcd_all = std::accumulate(nums.begin(), nums.end(), nums.front(), std::gcd<int, int>);
    const int lcm_all = std::accumulate(nums.begin(), nums.end(), nums.front(), std::lcm<int, int>);
    std::cout << "gcd(12,18,24)=" << gcd_all << "\n"
              << "lcm(12,18,24)=" << lcm_all << "\n";

    std::cout << "\n=== std::ratio ===\n";
    using half    = std::ratio<1, 2>;
    using quarter = std::ratio<1, 4>;
    using two_thirds = std::ratio<2, 3>;

    print_ratio<half>("half");
    print_ratio<quarter>("quarter");
    print_ratio<std::ratio<2, 4>>("2/4 (auto-simplified)");
    print_ratio<std::ratio_add<half, quarter>>("half+quarter");
    print_ratio<std::ratio_subtract<half, quarter>>("half-quarter");
    print_ratio<std::ratio_multiply<half, quarter>>("half*quarter");
    print_ratio<std::ratio_divide<half, quarter>>("half/quarter");
    print_ratio<two_thirds>("two_thirds");

    static_assert(std::ratio_equal_v<std::ratio<2, 4>, std::ratio<1, 2>>);
    static_assert(std::ratio_less_v<quarter, half>);

    return 0;
}
