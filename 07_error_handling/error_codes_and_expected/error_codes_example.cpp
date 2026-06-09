#include <iostream>
#include <string_view>
#include <vector>
#include <functional>
#include <concepts>
#include <cassert>
#include <span>
#include <stdexcept>

enum class ErrorCode { OK = 0, InvalidInput = 1, DivisionByZero = 2 };

[[nodiscard]] std::string_view to_string(ErrorCode e) noexcept {
    switch (e) {
        case ErrorCode::OK:             return "OK";
        case ErrorCode::InvalidInput:   return "InvalidInput";
        case ErrorCode::DivisionByZero: return "DivisionByZero";
    }
    return "Unknown";
}

class Result {
    int       value_ = 0;
    ErrorCode error_ = ErrorCode::OK;

public:
    Result() noexcept = default;

    explicit Result(int v)       noexcept : value_(v), error_(ErrorCode::OK) {}
    explicit Result(ErrorCode e) noexcept : value_(0), error_(e) {}

    [[nodiscard]] bool      is_ok()      const noexcept { return error_ == ErrorCode::OK; }
    [[nodiscard]] bool      has_error()  const noexcept { return !is_ok(); }
    [[nodiscard]] int       value()      const noexcept { return value_; }
    [[nodiscard]] ErrorCode error()      const noexcept { return error_; }

    [[nodiscard]] int unwrap_or(int fallback) const noexcept {
        return is_ok() ? value_ : fallback;
    }

    [[nodiscard]] int unwrap() const {
        if (!is_ok())
            throw std::runtime_error(std::string("unwrap on error: ") += to_string(error_));
        return value_;
    }

    template<std::invocable<int> Func>
    [[nodiscard]] Result map(Func f) const {
        if (is_ok()) return Result(std::invoke(f, value_));
        return Result(error_);
    }

    template<std::invocable<int> Func>
    [[nodiscard]] Result and_then(Func f) const {
        if (is_ok()) return std::invoke(f, value_);
        return Result(error_);
    }
};

void print_result(const Result& r) {
    if (r.is_ok())
        std::cout << "Ok("  << r.value() << ")\n";
    else
        std::cout << "Err(" << to_string(r.error()) << ")\n";
}

void print_results(std::span<const Result> results) {
    for (const auto& r : results) print_result(r);
}

[[nodiscard]] Result divide(int a, int b) noexcept {
    if (b == 0) return Result(ErrorCode::DivisionByZero);
    return Result(a / b);
}

[[nodiscard]] Result safe_add(int a, int b) noexcept {
    return Result(a + b);
}

[[nodiscard]] Result safe_subtract(int a, int b) noexcept {
    if (a < b) return Result(ErrorCode::InvalidInput);
    return Result(a - b);
}

[[nodiscard]] Result safe_multiply(int a, int b) noexcept {
    return Result(a * b);
}

[[nodiscard]] Result safe_modulo(int a, int b) noexcept {
    if (b == 0) return Result(ErrorCode::DivisionByZero);
    return Result(a % b);
}

[[nodiscard]] Result safe_square(int x) noexcept {
    return Result(x * x);
}

int main() {
    print_result(divide(10, 2));
    print_result(divide(10, 0));
    print_result(safe_add(5, 7));
    print_result(safe_subtract(10, 3));
    print_result(safe_subtract(3, 10));
    print_result(safe_multiply(4, 5));

    std::cout << "\n--- Chaining ---\n";
    print_result(divide(20, 2).and_then([](int v) { return safe_add(v, 5); }));

    std::cout << "\n--- Functional Style ---\n";
    print_result(divide(10, 2).map([](int x) noexcept { return x * 2; }));

    print_result(
        divide(20, 2)
            .and_then([](int v) { return safe_add(v, 10);      })
            .and_then([](int v) { return safe_multiply(v, 2);  })
    );

    print_result(
        divide(10, 0)
            .and_then([](int v) { return safe_add(v, 5); })
    );

    std::cout << "Fallback: " << divide(10, 0).unwrap_or(-1) << '\n';

    if (divide(10, 0).and_then([](int v){ return safe_add(v, 5); }).has_error())
        std::cout << "Error propagated correctly\n";

    std::cout << "\n--- Extra Utilities ---\n";
    print_result(safe_modulo(10, 3));
    print_result(safe_modulo(10, 0));

    const auto sq = safe_square(6);
    print_result(sq);
    std::cout << "Unwrapped: " << sq.unwrap() << '\n';

    const std::vector<Result> batch = {
        safe_add(1, 2),
        divide(9, 3),
        divide(5, 0),
        safe_subtract(20, 10),
    };
    std::cout << "\nBatch:\n";
    print_results(batch);

    auto process = [](const Result& r) {
        if (r.is_ok())
            std::cout << "Processed: " << r.value() * 10 << '\n';
        else
            std::cout << "Cannot process: " << to_string(r.error()) << '\n';
    };
    process(divide(10, 2));
    process(divide(10, 0));

    std::function<Result(int, int)> op = divide;
    print_result(op(50, 5));

    assert(divide(10, 2).is_ok());
    assert(divide(10, 0).has_error());
    assert(divide(10, 2).unwrap_or(-1) == 5);
    assert(safe_square(4).unwrap() == 16);
    assert(divide(20, 2).and_then([](int v){ return safe_multiply(v, 3); }).unwrap() == 30);

    std::cout << "\nAll assertions passed.\n";
    return 0;
}
