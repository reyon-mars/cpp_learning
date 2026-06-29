#include <iostream>
#include <vector>
#include <cassert>
#include <numeric>
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <ranges>
#include <concepts>
#include <span>

template<typename E>
class VectorExpr {
public:
    [[nodiscard]] double operator[](std::size_t i) const noexcept {
        return static_cast<const E&>(*this)[i];
    }
    [[nodiscard]] std::size_t size() const noexcept {
        return static_cast<const E&>(*this).size();
    }
};

class Vector : public VectorExpr<Vector> {
    std::vector<double> data_;

public:
    explicit Vector(std::size_t n) : data_(n, 0.0) {}

    template<typename E>
    explicit Vector(const VectorExpr<E>& expr) {
        const auto& e = static_cast<const E&>(expr);
        data_.resize(e.size());
        for (std::size_t i = 0; i < e.size(); ++i) data_[i] = e[i];
    }

    [[nodiscard]] double  operator[](std::size_t i) const noexcept { return data_[i]; }
    double&               operator[](std::size_t i)       noexcept { return data_[i]; }
    [[nodiscard]] std::size_t size() const noexcept { return data_.size(); }

    void fill(double value) noexcept { std::ranges::fill(data_, value); }

    void print(std::string_view label = "") const {
        if (!label.empty()) std::cout << label << ": ";
        for (double x : data_) std::cout << x << " ";
        std::cout << "\n";
    }

    [[nodiscard]] double dot(const Vector& other) const noexcept {
        assert(size() == other.size());
        return std::inner_product(data_.cbegin(), data_.cend(), other.data_.cbegin(), 0.0);
    }

    [[nodiscard]] double magnitude() const noexcept {
        return std::sqrt(std::inner_product(data_.cbegin(), data_.cend(), data_.cbegin(), 0.0));
    }

    [[nodiscard]] double normalize_magnitude() const noexcept {
        double mag = magnitude();
        return mag == 0.0 ? 0.0 : mag;
    }

    [[nodiscard]] Vector normalized() const {
        double mag = magnitude();
        assert(mag > 0.0);
        Vector result(size());
        for (std::size_t i = 0; i < size(); ++i) result[i] = data_[i] / mag;
        return result;
    }

    [[nodiscard]] double sum()         const noexcept { return std::accumulate(data_.cbegin(), data_.cend(), 0.0); }
    [[nodiscard]] double min_element() const noexcept { return *std::ranges::min_element(data_); }
    [[nodiscard]] double max_element() const noexcept { return *std::ranges::max_element(data_); }
    [[nodiscard]] double mean()        const noexcept { return size() ? sum() / static_cast<double>(size()) : 0.0; }

    [[nodiscard]] bool all_positive() const noexcept {
        return std::ranges::all_of(data_, [](double v) { return v > 0.0; });
    }

    [[nodiscard]] std::size_t count_positive() const noexcept {
        return static_cast<std::size_t>(std::ranges::count_if(data_, [](double v) { return v > 0.0; }));
    }

    [[nodiscard]] bool equal(const Vector& other, double tol = 1e-9) const noexcept {
        if (size() != other.size()) return false;
        for (std::size_t i = 0; i < size(); ++i)
            if (std::abs(data_[i] - other[i]) > tol) return false;
        return true;
    }
};

template<typename L, typename R>
class AddExpr : public VectorExpr<AddExpr<L, R>> {
    const L& left_;
    const R& right_;
public:
    AddExpr(const L& l, const R& r) noexcept : left_{l}, right_{r} {}
    [[nodiscard]] double      operator[](std::size_t i) const noexcept { return left_[i] + right_[i]; }
    [[nodiscard]] std::size_t size()                    const noexcept { return left_.size(); }
};

template<typename L, typename R>
class SubExpr : public VectorExpr<SubExpr<L, R>> {
    const L& left_;
    const R& right_;
public:
    SubExpr(const L& l, const R& r) noexcept : left_{l}, right_{r} {}
    [[nodiscard]] double      operator[](std::size_t i) const noexcept { return left_[i] - right_[i]; }
    [[nodiscard]] std::size_t size()                    const noexcept { return left_.size(); }
};

template<typename L, typename R>
class MulExpr : public VectorExpr<MulExpr<L, R>> {
    const L& left_;
    const R& right_;
public:
    MulExpr(const L& l, const R& r) noexcept : left_{l}, right_{r} {}
    [[nodiscard]] double      operator[](std::size_t i) const noexcept { return left_[i] * right_[i]; }
    [[nodiscard]] std::size_t size()                    const noexcept { return left_.size(); }
};

template<typename E>
class ScalarMultExpr : public VectorExpr<ScalarMultExpr<E>> {
    double   scalar_;
    const E& expr_;
public:
    ScalarMultExpr(double s, const E& e) noexcept : scalar_{s}, expr_{e} {}
    [[nodiscard]] double      operator[](std::size_t i) const noexcept { return scalar_ * expr_[i]; }
    [[nodiscard]] std::size_t size()                    const noexcept { return expr_.size(); }
};

template<typename E>
class ScalarAddExpr : public VectorExpr<ScalarAddExpr<E>> {
    double   scalar_;
    const E& expr_;
public:
    ScalarAddExpr(double s, const E& e) noexcept : scalar_{s}, expr_{e} {}
    [[nodiscard]] double      operator[](std::size_t i) const noexcept { return expr_[i] + scalar_; }
    [[nodiscard]] std::size_t size()                    const noexcept { return expr_.size(); }
};

template<typename E>
class ScalarSubExpr : public VectorExpr<ScalarSubExpr<E>> {
    double   scalar_;
    const E& expr_;
public:
    ScalarSubExpr(double s, const E& e) noexcept : scalar_{s}, expr_{e} {}
    [[nodiscard]] double      operator[](std::size_t i) const noexcept { return expr_[i] - scalar_; }
    [[nodiscard]] std::size_t size()                    const noexcept { return expr_.size(); }
};

template<typename E>
class NegateExpr : public VectorExpr<NegateExpr<E>> {
    const E& expr_;
public:
    explicit NegateExpr(const E& e) noexcept : expr_{e} {}
    [[nodiscard]] double      operator[](std::size_t i) const noexcept { return -expr_[i]; }
    [[nodiscard]] std::size_t size()                    const noexcept { return expr_.size(); }
};

template<typename L, typename R>
[[nodiscard]] AddExpr<L, R> operator+(const VectorExpr<L>& l, const VectorExpr<R>& r) noexcept {
    return {static_cast<const L&>(l), static_cast<const R&>(r)};
}

template<typename L, typename R>
[[nodiscard]] SubExpr<L, R> operator-(const VectorExpr<L>& l, const VectorExpr<R>& r) noexcept {
    return {static_cast<const L&>(l), static_cast<const R&>(r)};
}

template<typename L, typename R>
[[nodiscard]] MulExpr<L, R> operator*(const VectorExpr<L>& l, const VectorExpr<R>& r) noexcept {
    return {static_cast<const L&>(l), static_cast<const R&>(r)};
}

template<typename E>
[[nodiscard]] ScalarMultExpr<E> operator*(double s, const VectorExpr<E>& e) noexcept {
    return {s, static_cast<const E&>(e)};
}

template<typename E>
[[nodiscard]] ScalarMultExpr<E> operator*(const VectorExpr<E>& e, double s) noexcept {
    return {s, static_cast<const E&>(e)};
}

template<typename E>
[[nodiscard]] ScalarAddExpr<E> operator+(const VectorExpr<E>& e, double s) noexcept {
    return {s, static_cast<const E&>(e)};
}

template<typename E>
[[nodiscard]] ScalarSubExpr<E> operator-(const VectorExpr<E>& e, double s) noexcept {
    return {s, static_cast<const E&>(e)};
}

template<typename E>
[[nodiscard]] NegateExpr<E> operator-(const VectorExpr<E>& e) noexcept {
    return NegateExpr<E>{static_cast<const E&>(e)};
}

int main() {
    Vector v{5};
    for (std::size_t i = 0; i < 5; ++i) v[i] = static_cast<double>(i + 1);

    Vector v2{5};
    v2.fill(1.0);

    Vector result1{2.0 * v + v};
    result1.print("=== 2v + v ===");

    Vector result2{v - v2};
    result2.print("=== v - v2 ===");

    Vector result3{3.0 * v + v - v2};
    result3.print("=== 3v + v - v2 ===");

    Vector result4{v + 5.0};
    result4.print("=== v + 5.0 ===");

    Vector result5{v - 2.0};
    result5.print("=== v - 2.0 ===");

    Vector result6{2.0 * v + v2 - v + 3.0};
    result6.print("=== 2v + v2 - v + 3.0 ===");

    Vector result7{v * v};
    result7.print("=== v * v (element-wise) ===");

    Vector result8{Vector{-v}};
    result8.print("=== -v (negate) ===");

    Vector result9{v * 3.0};
    result9.print("=== v * 3.0 (scalar right) ===");

    std::cout << "\n=== Statistics ===\n"
              << "dot(v, v2)="    << v.dot(v2)        << "\n"
              << "magnitude="     << v.magnitude()     << "\n"
              << "sum="           << v.sum()            << "\n"
              << "mean="          << v.mean()           << "\n"
              << "min_element="   << v.min_element()    << "\n"
              << "max_element="   << v.max_element()    << "\n"
              << "all_positive="  << std::boolalpha << v.all_positive()    << "\n"
              << "count_positive="<< v.count_positive() << "\n";

    std::cout << "\n=== Normalized ===\n";
    Vector vn = v.normalized();
    vn.print("unit vector");
    std::cout << "magnitude after normalize=" << vn.magnitude() << "\n";

    std::cout << "\n=== Equality ===\n";
    Vector va{5}; va.fill(1.0);
    Vector vb{5}; vb.fill(1.0);
    std::cout << "va == vb? " << va.equal(vb) << "\n"
              << "va == v?  " << va.equal(v)  << "\n";

    assert(result1.size() == 5);
    assert(v.dot(v2) == 15.0);
    assert(va.equal(vb));
    assert(!va.equal(v));

    std::cout << "\nAll assertions passed.\n";
    return 0;
}
