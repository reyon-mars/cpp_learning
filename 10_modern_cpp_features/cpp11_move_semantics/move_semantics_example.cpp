#include <iostream>
#include <utility>
#include <algorithm>
#include <numeric>
#include <span>
#include <format>
#include <stdexcept>
#include <cassert>

class Vector {
    std::size_t size_{};
    int*        data_{};

public:
    explicit Vector(std::size_t s)
        : size_{s}, data_{new int[s]{}} {
        std::cout << std::format("Constructed (size={})\n", size_);
    }

    Vector(const Vector&)            = delete;
    Vector& operator=(const Vector&) = delete;

    Vector(Vector&& other) noexcept
        : size_{std::exchange(other.size_, 0)}
        , data_{std::exchange(other.data_, nullptr)} {
        std::cout << std::format("Move constructed (size={})\n", size_);
    }

    Vector& operator=(Vector&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            size_ = std::exchange(other.size_, 0);
            data_ = std::exchange(other.data_, nullptr);
            std::cout << std::format("Move assigned (size={})\n", size_);
        }
        return *this;
    }

    ~Vector() {
        delete[] data_;
    }

    [[nodiscard]] std::size_t size()     const noexcept { return size_; }
    [[nodiscard]] bool        empty()    const noexcept { return data_ == nullptr; }
    [[nodiscard]] bool        valid()    const noexcept { return data_ != nullptr; }

    [[nodiscard]] std::span<int>       as_span()       noexcept { return {data_, size_}; }
    [[nodiscard]] std::span<const int> as_span() const noexcept { return {data_, size_}; }

    int& operator[](std::size_t i) {
        assert(i < size_ && "index out of bounds");
        return data_[i];
    }

    const int& operator[](std::size_t i) const {
        assert(i < size_ && "index out of bounds");
        return data_[i];
    }

    [[nodiscard]] int*       begin()       noexcept { return data_; }
    [[nodiscard]] int*       end()         noexcept { return data_ + size_; }
    [[nodiscard]] const int* begin() const noexcept { return data_; }
    [[nodiscard]] const int* end()   const noexcept { return data_ + size_; }

    void fill(int value) noexcept {
        std::fill(begin(), end(), value);
    }

    void iota(int start = 0) noexcept {
        std::iota(begin(), end(), start);
    }

    [[nodiscard]] int sum() const noexcept {
        return std::reduce(begin(), end(), 0);
    }

    [[nodiscard]] double average() const noexcept {
        return size_ == 0 ? 0.0
                          : static_cast<double>(sum()) / static_cast<double>(size_);
    }

    [[nodiscard]] std::pair<int,int> minmax() const noexcept {
        if (size_ == 0) return {0, 0};
        const auto [lo, hi] = std::minmax_element(begin(), end());
        return {*lo, *hi};
    }

    void print_values() const {
        for (const int v : *this) std::cout << v << ' ';
        std::cout << '\n';
    }

    void print_state() const {
        std::cout << std::format("  size={} data={}\n",
                                 size_, static_cast<const void*>(data_));
    }
};

int main() {
    Vector v1{100};
    Vector v2 = std::move(v1);

    v2.print_state();
    std::cout << std::format("v1 after move: {}\n", v1.empty() ? "empty" : "not empty");

    Vector v3{50};
    v3 = std::move(v2);
    v3.print_state();

    v3.fill(5);
    std::cout << std::format("v3[0]: {}\n", v3[0]);
    std::cout << std::format("v2 size after move: {}\n", v2.size());
    std::cout << std::format("v3 empty? {}\n", v3.empty() ? "Yes" : "No");

    v3[0] = 99;
    std::cout << std::format("v3[0] after modify: {}\n", v3[0]);
    v3.print_state();

    Vector v4{10};
    v4 = std::move(v3);
    std::cout << "After moving v3 to v4:\n";
    v4.print_state();
    std::cout << std::format("v3 size: {}\n", v3.size());

    v4.fill(7);
    std::cout << "v4 values: ";
    v4.print_values();

    const auto [lo, hi] = v4.minmax();
    std::cout << std::format("sum={}  min={}  max={}  avg={}\n",
                             v4.sum(), lo, hi, v4.average());

    if (v4.size() > 1)
        std::swap(v4[0], v4[v4.size() - 1]);
    std::cout << "After swapping first/last: ";
    v4.print_values();

    Vector v5{8};
    v5.iota(1);
    std::cout << "v5 (iota from 1): ";
    v5.print_values();
    std::cout << std::format("v5 sum={} avg={}\n", v5.sum(), v5.average());

    Vector v6{std::move(v5)};
    std::cout << std::format("v5 valid after move? {}\n", v5.valid() ? "Yes" : "No");
    std::cout << std::format("v6 valid? {}\n", v6.valid() ? "Yes" : "No");

    auto sp = v6.as_span();
    std::for_each(sp.begin(), sp.end(), [](int& x){ x *= 2; });
    std::cout << "v6 doubled via span: ";
    v6.print_values();

    return 0;
}
