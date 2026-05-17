// Error Codes and Expected Exercise
// Alternative error handling patterns

#include <iostream>
#include <string>
#include <vector>      // 🔹 ADDED
#include <functional>  // 🔹 ADDED
#include <cassert>     // 🔹 ADDED

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

    // 🔹 EXTRA SMALL ADDITIONS

    // unwrap directly
    int unwrap() const {
        return value;
    }

    // reset result
    void reset() {
        value = 0;
        error = ErrorCode::OK;
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

// 🔹 EXTRA SMALL ADDITIONS

// safe division with validation
Result safe_modulo(int a, int b) {
    if (b == 0) {
        return Result(ErrorCode::DivisionByZero);
    }
    return Result(a % b);
}

// safe square
Result safe_square(int x) {
    return Result(x * x);
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

// 🔹 EXTRA: batch result printer
void print_results(const std::vector<Result>& results) {
    for (const auto& r : results) {
        print_result(r);
    }
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

    // ======================================================
    // 🔥 EXTRA SMALL ADDITIONS
    // ======================================================

    std::cout << "\n--- Extra Result Utilities ---\n";

    // modulo operation
    auto mod_result = safe_modulo(10, 3);
    print_result(mod_result);

    auto mod_fail = safe_modulo(10, 0);
    print_result(mod_fail);

    // square operation
    auto sq = safe_square(6);
    print_result(sq);

    // vector of results
    std::vector<Result> results = {
        safe_add(1, 2),
        divide(9, 3),
        divide(5, 0),
        safe_subtract(20, 10)
    };

    std::cout << "\nBatch results:\n";
    print_results(results);

    // lambda processor
    auto process_result = [](const Result& r) {
        if (r.is_ok())
            std::cout << "Processed value: "
                      << r.get_value() * 10 << "\n";
        else
            std::cout << "Cannot process error\n";
    };

    process_result(result);
    process_result(bad_result);

    // reset demo
    Result temp(100);
    print_result(temp);

    temp.reset();

    std::cout << "After reset:\n";
    print_result(temp);

    // assertion check
    assert(result.is_ok());
    assert(!bad_result.is_ok());

    // function wrapper example
    std::function<Result(int,int)> operation = divide;

    auto fn_result = operation(50, 5);
    print_result(fn_result);

    // unwrap demo
    if (sq.is_ok()) {
        std::cout << "Unwrapped square value: "
                  << sq.unwrap() << "\n";
    }

    // ======================================================

    return 0;
}
