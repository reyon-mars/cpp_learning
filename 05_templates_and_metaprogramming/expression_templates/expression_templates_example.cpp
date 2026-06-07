#include <iostream>
#include <vector>
#include <cassert>
#include <numeric>
#include <algorithm>
#include <cmath>
#include <cstddef>

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

    void fill(double value) noexcept { std::fill(data_.begin(), data_.end(), value); }

    void print() const {
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

    [[nodiscard]] double sum() const noexcept {
        return std::accumulate(data_.cbegin(), data_.cend(), 0.0);
    }

    [[nodiscard]] double max_element() const noexcept {
        return *std::max_element(data_.cbegin(), data_.cend());
    }

private:
    std::vector<double> data_;
};

template<typename L, typename R>
class AddExpr : public VectorExpr<AddExpr<L, R>> {
public:
    AddExpr(const L& l, const R& r) noexcept : left_{l}, right_{r} {}
    [[nodiscard]] double      operator[](std::size_t i) const noexcept { return left_[i] + right_[i]; }
    [[nodiscard]] std::size_t size()                    const noexcept { return left_.size(); }
private:
    const L& left_;
    const R& right_;
};

template<typename L, typename R>
class SubExpr : public VectorExpr<SubExpr<L, R>> {
public:
    SubExpr(const L& l, const R& r) noexcept : left_{l}, right_{r} {}
    [[nodiscard]] double      operator[](std::size_t i) const noexcept { return left_[i] - right_[i]; }
    [[nodiscard]] std::size_t size()                    const noexcept { return left_.size(); }
private:
    const L& left_;
    const R& right_;
};

template<typename E>
class ScalarMultExpr : public VectorExpr<ScalarMultExpr<E>> {
public:
    ScalarMultExpr(double s, const E& e) noexcept : scalar_{s}, expr_{e} {}
    [[nodiscard]] double      operator[](std::size_t i) const noexcept { return scalar_ * expr_[i]; }
    [[nodiscard]] std::size_t size()                    const noexcept { return expr_.size(); }
private:
    double   scalar_;
    const E& expr_;
};

template<typename E>
class ScalarAddExpr : public VectorExpr<ScalarAddExpr<E>> {
public:
    ScalarAddExpr(double s, const E& e) noexcept : scalar_{s}, expr_{e} {}
    [[nodiscard]] double      operator[](std::size_t i) const noexcept { return expr_[i] + scalar_; }
    [[nodiscard]] std::size_t size()                    const noexcept { return expr_.size(); }
private:
    double   scalar_;
    const E& expr_;
};

template<typename E>
class ScalarSubExpr : public VectorExpr<ScalarSubExpr<E>> {
public:
    ScalarSubExpr(double s, const E& e) noexcept : scalar_{s}, expr_{e} {}
    [[nodiscard]] double      operator[](std::size_t i) const noexcept { return expr_[i] - scalar_; }
    [[nodiscard]] std::size_t size()                    const noexcept { return expr_.size(); }
private:
    double   scalar_;
    const E& expr_;
};

template<typename L, typename R>
[[nodiscard]] AddExpr<L, R> operator+(const VectorExpr<L>& l, const VectorExpr<R>& r) noexcept {
    return {static_cast<const L&>(l), static_cast<const R&>(r)};
}

template<typename L, typename R>
[[nodiscard]] SubExpr<L, R> operator-(const VectorExpr<L>& l, const VectorExpr<R>& r) noexcept {
    return {static_cast<const L&>(l), static_cast<const R&>(r)};
}

template<typename E>
[[nodiscard]] ScalarMultExpr<E> operator*(double s, const VectorExpr<E>& e) noexcept {
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

int main() {
    Vector v{5};
    for (std::size_t i = 0; i < 5; ++i) v[i] = static_cast<double>(i + 1);

    std::cout << "=== 2v + v ===\n";
    Vector result1{2.0 * v + v};
    result1.print();

    std::cout << "\n=== v - v2 ===\n";
    Vector v2{5};
    v2.fill(1.0);
    Vector result2{v - v2};
    result2.print();

    std::cout << "\n=== 3v + v - v2 ===\n";
    Vector result3{3.0 * v + v - v2};
    result3.print();

    std::cout << "\n=== v + 5.0 (scalar add) ===\n";
    Vector result4{v + 5.0};
    result4.print();

    std::cout << "\n=== v - 2.0 (scalar sub) ===\n";
    Vector result5{v - 2.0};
    result5.print();

    std::cout << "\n=== 2v + v2 - v + 3.0 (chained) ===\n";
    Vector result6{2.0 * v + v2 - v + 3.0};
    result6.print();

    std::cout << "\n=== Statistics ===\n";
    std::cout << "dot(v, v2)="  << v.dot(v2)       << "\n"
              << "magnitude="   << v.magnitude()    << "\n"
              << "sum="         << v.sum()           << "\n"
              << "max_element=" << v.max_element()   << "\n";

    assert(result1.size() == 5);
    assert(v.dot(v2) == 15.0);

    return 0;
}
