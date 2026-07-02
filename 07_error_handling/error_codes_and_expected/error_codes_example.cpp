#include <iostream>
#include <string_view>
#include <string>
#include <vector>
#include <functional>
#include <concepts>
#include <cassert>
#include <span>
#include <stdexcept>
#include <optional>
#include <compare>
#include <type_traits>
#include <variant>
#include <cmath>

enum class ErrorCode { OK = 0, InvalidInput = 1, DivisionByZero = 2, Overflow = 3, OutOfRange = 4 };

[[nodiscard]] constexpr std::string_view to_string(ErrorCode e) noexcept {
    switch (e) {
        case ErrorCode::OK:             return "OK";
        case ErrorCode::InvalidInput:   return "InvalidInput";
        case ErrorCode::DivisionByZero: return "DivisionByZero";
        case ErrorCode::Overflow:       return "Overflow";
        case ErrorCode::OutOfRange:     return "OutOfRange";
    }
    return "Unknown";
}

std::ostream& operator<<(std::ostream& os, ErrorCode e) {
    return os << to_string(e);
}

template<typename T>
class Result {
    std::variant<T, ErrorCode> storage_{};

public:
    using value_type = T;

    Result() noexcept requires std::default_initializable<T> = default;

    Result(T v) noexcept(std::is_nothrow_move_constructible_v<T>) : storage_(std::move(v)) {}
    Result(ErrorCode e) noexcept : storage_(e) {}

    [[nodiscard]] static Result ok(T v) noexcept(std::is_nothrow_move_constructible_v<T>) {
        return Result(std::move(v));
    }
    [[nodiscard]] static Result err(ErrorCode e) noexcept { return Result(e); }

    [[nodiscard]] bool is_ok()     const noexcept { return std::holds_alternative<T>(storage_); }
    [[nodiscard]] bool has_error() const noexcept { return !is_ok(); }
    explicit operator bool()       const noexcept { return is_ok(); }

    [[nodiscard]] const T& value() const& {
        if (!is_ok()) throw std::runtime_error(std::string("value on error: ").append(to_string(error())));
        return std::get<T>(storage_);
    }
    [[nodiscard]] T&& value() && {
        if (!is_ok()) throw std::runtime_error(std::string("value on error: ").append(to_string(error())));
        return std::get<T>(std::move(storage_));
    }

    [[nodiscard]] ErrorCode error() const noexcept {
        return is_ok() ? ErrorCode::OK : std::get<ErrorCode>(storage_);
    }

    [[nodiscard]] T unwrap() const { return value(); }

    [[nodiscard]] T value_or(T fallback) const
        noexcept(std::is_nothrow_copy_constructible_v<T>) {
        return is_ok() ? std::get<T>(storage_) : fallback;
    }
    [[nodiscard]] T unwrap_or(T fallback) const
        noexcept(std::is_nothrow_copy_constructible_v<T>) {
        return value_or(fallback);
    }

    [[nodiscard]] std::optional<T> to_optional() const
        noexcept(std::is_nothrow_copy_constructible_v<T>) {
        if (is_ok()) return std::get<T>(storage_);
        return std::nullopt;
    }

    template<std::invocable<const T&> Func>
    [[nodiscard]] auto map(Func f) const -> Result<std::invoke_result_t<Func, const T&>> {
        using U = std::invoke_result_t<Func, const T&>;
        if (is_ok()) return Result<U>::ok(std::invoke(f, std::get<T>(storage_)));
        return Result<U>::err(error());
    }

    template<std::invocable<const T&> Func>
    [[nodiscard]] auto and_then(Func f) const -> std::invoke_result_t<Func, const T&> {
        using R = std::invoke_result_t<Func, const T&>;
        static_assert(std::is_same_v<typename R::value_type, typename R::value_type>);
        if (is_ok()) return std::invoke(f, std::get<T>(storage_));
        return R::err(error());
    }

    template<std::invocable<ErrorCode> Func>
    [[nodiscard]] Result or_else(Func f) const {
        if (is_ok()) return *this;
        return std::invoke(f, error());
    }

    template<std::predicate<const T&> Pred>
    [[nodiscard]] Result filter(Pred p, ErrorCode on_fail) const {
        if (is_ok() && !std::invoke(p, std::get<T>(storage_))) return Result::err(on_fail);
        return *this;
    }

    [[nodiscard]] bool operator==(const Result& other) const {
        return storage_ == other.storage_;
    }
};

template<typename T>
std::ostream& operator<<(std::ostream& os, const Result<T>& r) {
    if (r.is_ok()) os << "Ok(" << r.value() << ")";
    else           os << "Err(" << r.error() << ")";
    return os;
}

template<typename T>
void print_result(const Result<T>& r) {
    std::cout << r << '\n';
}

template<typename T>
void print_results(std::span<const Result<T>> results) {
    for (const auto& r : results) print_result(r);
}

[[nodiscard]] Result<int> divide(int a, int b) noexcept {
    if (b == 0) return Result<int>::err(ErrorCode::DivisionByZero);
    return Result<int>::ok(a / b);
}

[[nodiscard]] Result<int> safe_add(int a, int b) noexcept {
    return Result<int>::ok(a + b);
}

[[nodiscard]] Result<int> safe_subtract(int a, int b) noexcept {
    if (a < b) return Result<int>::err(ErrorCode::InvalidInput);
    return Result<int>::ok(a - b);
}

[[nodiscard]] Result<int> safe_multiply(int a, int b) noexcept {
    return Result<int>::ok(a * b);
}

[[nodiscard]] Result<int> safe_modulo(int a, int b) noexcept {
    if (b == 0) return Result<int>::err(ErrorCode::DivisionByZero);
    return Result<int>::ok(a % b);
}

[[nodiscard]] Result<int> safe_square(int x) noexcept {
    return Result<int>::ok(x * x);
}

[[nodiscard]] Result<double> safe_sqrt(double x) noexcept {
    if (x < 0.0) return Result<double>::err(ErrorCode::InvalidInput);
    return Result<double>::ok(std::sqrt(x));
}

[[nodiscard]] Result<int> safe_at(std::span<const int> data, std::size_t index) noexcept {
    if (index >= data.size()) return Result<int>::err(ErrorCode::OutOfRange);
    return Result<int>::ok(data[index]);
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

    std::cout << "\n--- or_else / filter ---\n";
    print_result(divide(10, 0).or_else([](ErrorCode) { return safe_add(0, 0); }));
    print_result(safe_add(4, 4).filter([](int v) { return v % 2 == 0; }, ErrorCode::InvalidInput));
    print_result(safe_add(4, 3).filter([](int v) { return v % 2 == 0; }, ErrorCode::InvalidInput));

    std::cout << "\n--- Extra Utilities ---\n";
    print_result(safe_modulo(10, 3));
    print_result(safe_modulo(10, 0));

    const auto sq = safe_square(6);
    print_result(sq);
    std::cout << "Unwrapped: " << sq.unwrap() << '\n';

    print_result(safe_sqrt(16.0));
    print_result(safe_sqrt(-4.0));

    if (const auto opt = safe_sqrt(9.0).to_optional(); opt.has_value())
        std::cout << "Optional sqrt: " << *opt << '\n';

    const std::vector<int> data{10, 20, 30};
    print_result(safe_at(data, 1));
    print_result(safe_at(data, 9));

    const std::vector<Result<int>> batch = {
        safe_add(1, 2),
        divide(9, 3),
        divide(5, 0),
        safe_subtract(20, 10),
    };
    std::cout << "\nBatch:\n";
    print_results<int>(batch);

    auto process = [](const Result<int>& r) {
        if (r.is_ok())
            std::cout << "Processed: " << r.value() * 10 << '\n';
        else
            std::cout << "Cannot process: " << r.error() << '\n';
    };
    process(divide(10, 2));
    process(divide(10, 0));

    std::function<Result<int>(int, int)> op = divide;
    print_result(op(50, 5));

    assert(divide(10, 2).is_ok());
    assert(divide(10, 0).has_error());
    assert(divide(10, 2).unwrap_or(-1) == 5);
    assert(safe_square(4).unwrap() == 16);
    assert(divide(20, 2).and_then([](int v){ return safe_multiply(v, 3); }).unwrap() == 30);

    assert(safe_add(2, 2) == Result<int>::ok(4));
    assert(divide(1, 0) == Result<int>::err(ErrorCode::DivisionByZero));

    std::cout << "\nAll assertions passed.\n";
    return 0;
}
