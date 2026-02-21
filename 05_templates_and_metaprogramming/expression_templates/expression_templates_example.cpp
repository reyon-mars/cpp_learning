// Expression Templates Exercise
// Lazy evaluation and DSLs

#include <iostream>
#include <vector>

// Scalar-vector product expression
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

class Vector : public VectorExpr<Vector> {
private:
    std::vector<double> data;
    
public:
    Vector(size_t n) : data(n) {}
    
    double operator[](size_t i) const { return data[i]; }
    double& operator[](size_t i) { return data[i]; }
    size_t size() const { return data.size(); }
};

// Scalar multiplication expression
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

int main() {
    Vector v(5);
    v[0] = 1; v[1] = 2; v[2] = 3; v[3] = 4; v[4] = 5;
    
    auto expr = ScalarMultExpr<Vector>(2.0, v);
    
    for (size_t i = 0; i < expr.size(); ++i) {
        std::cout << expr[i] << " ";
    }
    std::cout << "\n";
    
    return 0;
}
