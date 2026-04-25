// Expression Templates Exercise
// Lazy evaluation and DSLs

#include <iostream>
#include <vector>
#include <cassert>   // ✅ ADDED

// ----------------------------------
// Base Expression Template
// ----------------------------------
template<typename E>
class VectorExpr {
public:
    double operator[](size_t i) const {
        return static_cast<const E&>(*this)[i];
    }

    size_t size() const {
        return static_cast<const E&>(*this).size();
    }
};

// ----------------------------------
// Concrete Vector
// ----------------------------------
class Vector : public VectorExpr<Vector> {
private:
    std::vector<double> data;

public:
    Vector(size_t n) : data(n) {}

    // Construct from expression (lazy evaluation happens here)
    template<typename E>
    Vector(const VectorExpr<E>& expr) {
        const E& e = static_cast<const E&>(expr);
        data.resize(e.size());
        for (size_t i = 0; i < e.size(); ++i)
            data[i] = e[i];
    }

    double operator[](size_t i) const { return data[i]; }
    double& operator[](size_t i) { return data[i]; }
    size_t size() const { return data.size(); }

    // -------- SMALL ADDITIONS --------

    void fill(double value) {
        for (auto& x : data)
            x = value;
    }

    void print() const {
        for (double x : data)
            std::cout << x << " ";
        std::cout << "\n";
    }

    // ✅ ADDED: dot product
    double dot(const Vector& other) const {
        assert(size() == other.size());
        double result = 0;
        for (size_t i = 0; i < size(); ++i)
            result += data[i] * other.data[i];
        return result;
    }

    // --------------------------------
};

// ----------------------------------
// Scalar Multiplication Expression
// ----------------------------------
template<typename E>
class ScalarMultExpr : public VectorExpr<ScalarMultExpr<E>> {
private:
    double scalar;
    const E& expr;

public:
    ScalarMultExpr(double s, const E& e) : scalar(s), expr(e) {}

    double operator[](size_t i) const { return scalar * expr[i]; }
    size_t size() const { return expr.size(); }
};

// ----------------------------------
// Vector Addition Expression
// ----------------------------------
template<typename L, typename R>
class AddExpr : public VectorExpr<AddExpr<L, R>> {
private:
    const L& left;
    const R& right;

public:
    AddExpr(const L& l, const R& r) : left(l), right(r) {}

    double operator[](size_t i) const {
        return left[i] + right[i];
    }

    size_t size() const {
        return left.size();
    }
};

// ----------------------------------
// Operator Overloads (DSL-style)
// ----------------------------------
template<typename E>
ScalarMultExpr<E> operator*(double scalar, const VectorExpr<E>& expr) {
    return ScalarMultExpr<E>(scalar, static_cast<const E&>(expr));
}

template<typename L, typename R>
AddExpr<L, R> operator+(const VectorExpr<L>& l,
                        const VectorExpr<R>& r) {
    return AddExpr<L, R>(
        static_cast<const L&>(l),
        static_cast<const R&>(r)
    );
}

// ---------------- SMALL ADDITIONS ----------------

// Vector subtraction expression
template<typename L, typename R>
class SubExpr : public VectorExpr<SubExpr<L, R>> {
private:
    const L& left;
    const R& right;

public:
    SubExpr(const L& l, const R& r) : left(l), right(r) {}

    double operator[](size_t i) const {
        return left[i] - right[i];
    }

    size_t size() const {
        return left.size();
    }
};

template<typename L, typename R>
SubExpr<L, R> operator-(const VectorExpr<L>& l,
                        const VectorExpr<R>& r) {
    return SubExpr<L, R>(
        static_cast<const L&>(l),
        static_cast<const R&>(r)
    );
}

// ✅ ADDED: scalar addition expression
template<typename E>
class ScalarAddExpr : public VectorExpr<ScalarAddExpr<E>> {
private:
    double scalar;
    const E& expr;

public:
    ScalarAddExpr(double s, const E& e) : scalar(s), expr(e) {}

    double operator[](size_t i) const {
        return expr[i] + scalar;
    }

    size_t size() const {
        return expr.size();
    }
};

template<typename E>
ScalarAddExpr<E> operator+(const VectorExpr<E>& expr, double scalar) {
    return ScalarAddExpr<E>(scalar, static_cast<const E&>(expr));
}

// ------------------------------------------------

// ----------------------------------
// Main
// ----------------------------------
int main() {

    Vector v(5);
    for (size_t i = 0; i < 5; ++i)
        v[i] = i + 1;  // 1 2 3 4 5

    auto expr = 2.0 * v + v;

    Vector result = expr;

    result.print();

    // -------- ADDED USAGE --------

    Vector v2(5);
    v2.fill(1.0);

    auto expr2 = v - v2;
    Vector result2 = expr2;

    std::cout << "After subtraction:\n";
    result2.print();

    auto expr3 = 3.0 * v + v - v2;
    Vector result3 = expr3;

    std::cout << "Complex expression:\n";
    result3.print();

    // ✅ NEW: scalar addition
    auto expr4 = v + 5.0;
    Vector result4 = expr4;

    std::cout << "After scalar addition:\n";
    result4.print();

    // ✅ NEW: dot product
    std::cout << "Dot product (v · v2): "
              << v.dot(v2) << "\n";

    // ----------------------------

    return 0;
}
