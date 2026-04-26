// Higher-Order Functions Exercise
// Functions that work with other functions

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <functional>

int add(int a, int b) { return a + b; }
int multiply(int a, int b) { return a * b; }

// ----------------------------------
// Higher-order function: applies a binary operation
// ----------------------------------
template<typename BinOp>
int reduce(const std::vector<int>& values, BinOp op) {
    int result = values[0];
    for (size_t i = 1; i < values.size(); ++i) {
        result = op(result, values[i]);
    }
    return result;
}

// ----------------------------------
// Generic reduce (type-independent)
// ----------------------------------
template<typename T, typename BinOp>
T reduce_generic(const std::vector<T>& values, BinOp op) {
    T result = values[0];
    for (size_t i = 1; i < values.size(); ++i) {
        result = op(result, values[i]);
    }
    return result;
}

// ----------------------------------
// Function returning a function
// ----------------------------------
std::function<int(int)> make_multiplier(int factor) {
    return [factor](int x) {
        return x * factor;
    };
}

// 🔹 NEW: Higher-order filter function
template<typename Pred>
std::vector<int> filter(const std::vector<int>& values, Pred p) {
    std::vector<int> result;
    for (int v : values) {
        if (p(v)) result.push_back(v);
    }
    return result;
}

// 🔹 NEW: Apply function to each element
template<typename Func>
void for_each_apply(std::vector<int>& values, Func f) {
    for (auto& v : values) {
        v = f(v);
    }
}

// ======================================================
// 🔥 NEW ADDITIONS (ADVANCED BUT SMALL)
// ======================================================

// 🔹 map (functional transform)
template<typename Func>
std::vector<int> map(const std::vector<int>& values, Func f) {
    std::vector<int> result(values.size());
    std::transform(values.begin(), values.end(), result.begin(), f);
    return result;
}

// 🔹 compose two functions: f(g(x))
template<typename F, typename G>
auto compose(F f, G g) {
    return [=](auto x) {
        return f(g(x));
    };
}

// 🔹 pipeline-style chaining
template<typename T, typename Func>
auto pipe(T value, Func f) {
    return f(value);
}

// 🔹 combine predicates (AND)
template<typename P1, typename P2>
auto and_predicate(P1 p1, P2 p2) {
    return [=](int x) {
        return p1(x) && p2(x);
    };
}

// 🔹 safe reduce (handles empty vector)
template<typename BinOp>
int safe_reduce(const std::vector<int>& values, BinOp op, int init = 0) {
    if (values.empty()) return init;
    return reduce(values, op);
}

// ======================================================
// Main
// ======================================================

int main() {

    std::vector<int> values = {1, 2, 3, 4, 5};

    int sum = reduce(values, add);
    int product = reduce(values, multiply);

    std::cout << "Sum: " << sum << "\n";
    std::cout << "Product: " << product << "\n";

    int max_val = reduce(values,
        [](int a, int b) { return std::max(a, b); });

    std::cout << "Max: " << max_val << "\n";

    double avg = reduce_generic<double>(
        {1.0, 2.0, 3.0},
        [](double a, double b) { return a + b; }
    ) / 3.0;

    std::cout << "Average: " << avg << "\n";

    int std_sum = std::accumulate(
        values.begin(), values.end(), 0);

    std::cout << "std::accumulate sum: "
              << std_sum << "\n";

    std::vector<int> doubled(values.size());
    std::transform(values.begin(),
                   values.end(),
                   doubled.begin(),
                   make_multiplier(2));

    std::cout << "Doubled values: ";
    for (int v : doubled)
        std::cout << v << " ";
    std::cout << "\n";

    auto evens = filter(values, [](int x) { return x % 2 == 0; });
    std::cout << "Even values: ";
    for (int v : evens)
        std::cout << v << " ";
    std::cout << "\n";

    for_each_apply(values, [](int x) { return x * x; });
    std::cout << "Squared values: ";
    for (int v : values)
        std::cout << v << " ";
    std::cout << "\n";

    int custom = reduce(values, [](int a, int b) {
        return a + b * 2;
    });
    std::cout << "Custom reduce result: " << custom << "\n";

    // ======================================================
    // 🔥 NEW USAGE
    // ======================================================

    std::cout << "\n--- Advanced Functional Patterns ---\n";

    // map
    auto tripled = map(values, [](int x) { return x * 3; });
    std::cout << "Tripled: ";
    for (int v : tripled) std::cout << v << " ";
    std::cout << "\n";

    // compose
    auto square = [](int x) { return x * x; };
    auto double_then_square = compose(square, make_multiplier(2));

    std::cout << "compose(double->square)(5): "
              << double_then_square(5) << "\n";

    // pipeline
    int piped = pipe(5, make_multiplier(3));
    std::cout << "Pipe result: " << piped << "\n";

    // combined predicate
    auto even_and_gt10 = and_predicate(
        [](int x){ return x % 2 == 0; },
        [](int x){ return x > 10; }
    );

    auto filtered = filter(values, even_and_gt10);
    std::cout << "Even and >10: ";
    for (int v : filtered) std::cout << v << " ";
    std::cout << "\n";

    // safe reduce
    std::vector<int> empty;
    std::cout << "Safe reduce (empty): "
              << safe_reduce(empty, add, 0) << "\n";

    // ======================================================

    return 0;
}
