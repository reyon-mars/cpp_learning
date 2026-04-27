// Error Codes and Expected Exercise
// Alternative error handling patterns

#include <iostream>
#include <string>

enum class ErrorCode {
    OK = 0,
    InvalidInput = 1,
    DivisionByZero = 2
};

class Result {
private:
    int value;
    ErrorCode error;
    
public:
    Result(int v) : value(v), error(ErrorCode::OK) {}
    Result(ErrorCode e) : value(0), error(e) {}
    
    bool is_ok() const { return error == ErrorCode::OK; }
    int get_value() const { return value; }
    ErrorCode get_error() const { return error; }

    // -------- NEW ADDITIONS --------

    bool has_error() const { return !is_ok(); }

    // map: apply function if OK
    template<typename Func>
    Result map(Func f) const {
        if (is_ok()) return Result(f(value));
        return Result(error);
    }

    // and_then: chain operations returning Result
    template<typename Func>
    Result and_then(Func f) const {
        if (is_ok()) return f(value);
        return Result(error);
    }

    // unwrap with default
    int unwrap_or(int default_val) const {
        return is_ok() ? value : default_val;
    }

    // --------------------------------
};

// Helper: convert error to string
std::string to_string(ErrorCode e) {
    switch (e) {
        case ErrorCode::OK: return "OK";
        case ErrorCode::InvalidInput: return "Invalid Input";
        case ErrorCode::DivisionByZero: return "Division By Zero";
        default: return "Unknown Error";
    }
}

// Example operation
Result divide(int a, int b) {
    if (b == 0) {
        return Result(ErrorCode::DivisionByZero);
    }
    return Result(a / b);
}

// Another operation
Result safe_add(int a, int b) {
    return Result(a + b);
}

// 🔹 NEW: safe subtraction with validation
Result safe_subtract(int a, int b) {
    if (a < b) {
        return Result(ErrorCode::InvalidInput);
    }
    return Result(a - b);
}

// 🔹 NEW: safe multiplication
Result safe_multiply(int a, int b) {
    return Result(a * b);
}

// Helper printer
void print_result(const Result& r) {
    if (r.is_ok()) {
        std::cout << "Result: " << r.get_value() << "\n";
    } else {
        std::cout << "Error: " << to_string(r.get_error()) << "\n";
    }
}

// 🔹 NEW: check and print in one line
void quick_check(const Result& r) {
    std::cout << (r.is_ok() ? "OK -> " : "ERR -> ");
    print_result(r);
}

int main() {

    auto result = divide(10, 2);
    print_result(result);

    auto bad_result = divide(10, 0);
    print_result(bad_result);

    // Additional example
    auto add_result = safe_add(5, 7);
    print_result(add_result);

    // 🔹 NEW: subtraction demo
    auto sub_ok = safe_subtract(10, 3);
    auto sub_bad = safe_subtract(3, 10);
    print_result(sub_ok);
    print_result(sub_bad);

    // 🔹 NEW: multiplication demo
    auto mul_result = safe_multiply(4, 5);
    quick_check(mul_result);

    // 🔹 NEW: chaining style usage
    auto chained = divide(20, 2);
    if (chained.is_ok()) {
        auto next = safe_add(chained.get_value(), 5);
        print_result(next);
    }

    // -------- NEW ADVANCED USAGE --------

    std::cout << "\n--- Functional Style Handling ---\n";

    // map example
    auto mapped = divide(10, 2).map([](int x) { return x * 2; });
    print_result(mapped);

    // and_then chaining
    auto chained2 = divide(20, 2)
        .and_then([](int v) { return safe_add(v, 10); })
        .and_then([](int v) { return safe_multiply(v, 2); });

    print_result(chained2);

    // error propagation
    auto fail_chain = divide(10, 0)
        .and_then([](int v) { return safe_add(v, 5); });

    print_result(fail_chain);

    // unwrap_or demo
    int safe_val = divide(10, 0).unwrap_or(-1);
    std::cout << "Fallback value: " << safe_val << "\n";

    // has_error demo
    if (fail_chain.has_error()) {
        std::cout << "Detected error in chain\n";
    }

    // -----------------------------------

    return 0;
}
