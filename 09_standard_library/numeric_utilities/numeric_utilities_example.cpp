#include <iostream>
#include <complex>
#include <numeric>
#include <ratio>
#include <vector>
#include <string_view>
#include <cmath>
#include <chrono>
#include <numbers>

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

[[nodiscard]] std::vector<std::complex<double>> nth_roots_of_unity(int n) {
    std::vector<std::complex<double>> roots;
    roots.reserve(n);
    for (int k = 0; k < n; ++k)
        roots.push_back(std::polar(1.0, 2.0 * std::numbers::pi * k / n));
    return roots;
}

int main() {
    std::cout << "=== Complex numbers ===\n";
    const std::complex<double> z1{3.0, 4.0};
    const std::complex<double> z2{1.0, 2.0};
    print_complex(z1, "z1");
    print_complex(z2, "z2");
    std::cout << "z1+z2="     << z1 + z2                << "\n"
              << "z1*z2="     << z1 * z2                << "\n"
              << "z1/z2="     << z1 / z2                << "\n"
              << "z1^2="      << std::pow(z1, 2)        << "\n"
              << "sqrt(z1)="  << std::sqrt(z1)          << "\n"
              << "exp(z2)="   << std::exp(z2)           << "\n"
              << "log(z1)="   << std::log(z1)           << "\n"
              << "sin(z2)="   << std::sin(z2)           << "\n"
              << "cos(z2)="   << std::cos(z2)           << "\n"
              << "polar(5,0.5)=" << std::polar(5.0, 0.5) << "\n"
              << "z1==z1="    << std::boolalpha << (z1 == z1) << "\n"
              << "z1==z2="    << (z1 == z2)                   << "\n";

    std::cout << "\n=== Roots of unity ===\n";
    for (auto n : {3, 4}) {
        std::cout << n << "th roots: ";
        for (const auto& root : nth_roots_of_unity(n))
            std::cout << root << " ";
        std::cout << "\n";
    }

    std::cout << "\n=== GCD / LCM ===\n";
    std::cout << std::boolalpha
              << "gcd(48,18)="  << std::gcd(48, 18)   << "\n"
              << "lcm(48,18)="  << std::lcm(48, 18)   << "\n"
              << "gcd(21,6)="   << std::gcd(21,  6)   << "\n"
              << "lcm(21,6)="   << std::lcm(21,  6)   << "\n"
              << "coprime(35,64)=" << are_coprime(35, 64) << "\n"
              << "coprime(12,18)=" << are_coprime(12, 18) << "\n";

    std::cout << "\n=== GCD / LCM over vector ===\n";
    const std::vector<int> nums{12, 18, 24};
    const int gcd_all = std::accumulate(nums.begin(), nums.end(), nums.front(), std::gcd<int, int>);
    const int lcm_all = std::accumulate(nums.begin(), nums.end(), nums.front(), std::lcm<int, int>);
    std::cout << "gcd(12,18,24)=" << gcd_all << "\n"
              << "lcm(12,18,24)=" << lcm_all << "\n";

    std::cout << "\n=== midpoint / lerp ===\n";
    std::cout << "midpoint(4,10)="   << std::midpoint(4, 10)      << "\n"
              << "midpoint(-3,7)="   << std::midpoint(-3, 7)      << "\n"
              << "lerp(0,10,0.25)="  << std::lerp(0.0, 10.0, 0.25) << "\n"
              << "lerp(0,10,1.5)="   << std::lerp(0.0, 10.0, 1.5)  << "\n";

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
    static_assert(std::ratio_greater_v<half, quarter>);
    static_assert(std::ratio_less_equal_v<half, half>);
    static_assert(std::ratio_greater_equal_v<half, quarter>);

    std::cout << "\n=== std::ratio driving std::chrono::duration ===\n";
    using centiseconds = std::chrono::duration<long long, std::centi>;
    using two_fifths_second = std::chrono::duration<double, std::ratio<2, 5>>;
    const auto cs = centiseconds{250};
    const auto as_seconds = std::chrono::duration<double>{cs};
    const two_fifths_second custom_unit{1};
    std::cout << "250 centiseconds = " << as_seconds.count() << " s\n"
              << "1 unit of ratio<2,5> seconds = "
              << std::chrono::duration<double>{custom_unit}.count() << " s\n";

    return 0;
}
