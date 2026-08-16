#include <iostream>
#include <exception>
#include <string>
#include <string_view>
#include <stdexcept>
#include <vector>
#include <functional>
#include <concepts>
#include <cassert>
#include <optional>
#include <variant>
#include <source_location>
#include <format>
#include <span>
#include <type_traits>
#include <thread>
#include <mutex>
#include <future>

class MyException : public std::exception {
    std::string          message_;
    std::source_location location_;
public:
    explicit MyException(std::string msg,
                         std::source_location loc = std::source_location::current())
        : message_(std::move(msg)), location_(loc) {}

    [[nodiscard]] const char*                 what()  const noexcept override { return message_.c_str(); }
    [[nodiscard]] const std::source_location& where() const noexcept { return location_; }

    [[nodiscard]] std::string diagnostic() const {
        return std::format("[{}:{}] {}", location_.file_name(), location_.line(), message_);
    }
};

class ValidationError : public MyException {
public:
    explicit ValidationError(std::string_view msg,
                             std::source_location loc = std::source_location::current())
        : MyException(std::format("ValidationError: {}", msg), loc) {}
};

class MathError : public MyException {
public:
    explicit MathError(std::string_view msg,
                       std::source_location loc = std::source_location::current())
        : MyException(std::format("MathError: {}", msg), loc) {}
};

class FileError : public MyException {
public:
    explicit FileError(std::string_view msg,
                       std::source_location loc = std::source_location::current())
        : MyException(std::format("FileError: {}", msg), loc) {}
};

class NetworkError : public MyException {
public:
    explicit NetworkError(std::string_view msg,
                          std::source_location loc = std::source_location::current())
        : MyException(std::format("NetworkError: {}", msg), loc) {}
};

class DatabaseError : public MyException {
public:
    explicit DatabaseError(std::string_view msg,
                           std::source_location loc = std::source_location::current())
        : MyException(std::format("DatabaseError: {}", msg), loc) {}
};

template <typename T, typename E = MyException>
using Result = std::variant<T, E>;

template <typename T, typename E>
[[nodiscard]] bool is_ok(const Result<T, E>& r) noexcept {
    return std::holds_alternative<T>(r);
}

template <typename T, typename E>
[[nodiscard]] const T& unwrap(const Result<T, E>& r) {
    if (!is_ok(r)) throw std::get<E>(r);
    return std::get<T>(r);
}

template <typename T, typename E>
void print_result(const Result<T, E>& r, std::string_view label = "") {
    if (!label.empty()) std::cout << label << ": ";
    if (is_ok(r))
        std::cout << std::format("Ok({})\n",  unwrap(r));
    else
        std::cout << std::format("Err({})\n", std::get<E>(r).what());
}

[[nodiscard]] int divide(int a, int b) {
    if (b == 0) throw std::invalid_argument("Division by zero");
    return a / b;
}

[[nodiscard]] Result<int, std::invalid_argument> divide_result(int a, int b) noexcept {
    if (b == 0) return std::invalid_argument("Division by zero");
    return a / b;
}

void validate_number(int n) {
    if (n < 0) throw ValidationError("Negative numbers not allowed");
}

void check_positive(int n) {
    if (n <= 0) throw ValidationError("Value must be positive");
}

[[nodiscard]] int safe_square(int n) {
    if (n < 0) throw MathError("Square root of negative number");
    return n * n;
}

void open_file(std::string_view filename) {
    if (filename.empty()) throw FileError("Filename is empty");
    std::cout << std::format("File opened: {}\n", filename);
}

void connect_server(bool success) {
    if (!success) throw NetworkError("Unable to connect to server");
    std::cout << "Connected to server\n";
}

void query_database(bool success) {
    if (!success) throw DatabaseError("Database query failed");
    std::cout << "Database query executed\n";
}

void wrapper_divide(int a, int b) {
    try {
        std::cout << std::format("Wrapper result: {}\n", divide(a, b));
    } catch (...) {
        std::cout << "Exception in wrapper, rethrowing\n";
        throw;
    }
}

[[nodiscard]] std::optional<int> try_divide(int a, int b) noexcept {
    if (b == 0) return std::nullopt;
    return a / b;
}

void nested_example() {
    try {
        throw MathError("Inner math failure");
    } catch (...) {
        std::throw_with_nested(MyException("Outer exception with nested cause"));
    }
}

void print_nested(const std::exception& e, int level = 0) {
    std::cout << std::format("{:{}}{}\n", "",
                             static_cast<std::size_t>(level) * 2,
                             std::format("Exception: {}", e.what()));
    try {
        std::rethrow_if_nested(e);
    } catch (const std::exception& nested) {
        print_nested(nested, level + 1);
    } catch (...) {}
}

void no_throw_function() noexcept {
    std::cout << "Guaranteed no exceptions\n";
}

template <std::invocable Fn>
void execute_safely(Fn&& fn) {
    try {
        std::invoke(std::forward<Fn>(fn));
    } catch (const std::exception& e) {
        std::cout << std::format("Handled in execute_safely: {}\n", e.what());
    }
}

template <std::invocable Fn>
[[nodiscard]] std::optional<std::string> capture_exception(Fn&& fn) noexcept {
    try {
        std::invoke(std::forward<Fn>(fn));
        return std::nullopt;
    } catch (const std::exception& e) {
        return std::string(e.what());
    } catch (...) {
        return std::string("unknown exception");
    }
}

class ExceptionLogger {
    std::vector<std::string> logs_;
public:
    template <std::invocable Fn>
    void log_exception(Fn&& fn) {
        if (auto msg = capture_exception(std::forward<Fn>(fn)))
            logs_.emplace_back(std::move(*msg));
    }

    [[nodiscard]] std::span<const std::string> logs()  const noexcept { return logs_; }
    [[nodiscard]] std::size_t                  size()  const noexcept { return logs_.size(); }

    void print_all() const {
        for (const auto& entry : logs_)
            std::cout << std::format("- {}\n", entry);
    }
};

template <std::invocable Fn>
void with_retry(int attempts, Fn&& fn, std::string_view label = "operation") {
    for (int i = 0; i < attempts; ++i) {
        try {
            std::invoke(std::forward<Fn>(fn));
            return;
        } catch (const std::exception& e) {
            std::cout << std::format("Attempt {}/{} failed: {}\n", i + 1, attempts, e.what());
            if (i + 1 == attempts)
                std::cout << std::format("{} exhausted all {} attempts\n", label, attempts);
        }
    }
}

enum class ErrorCode { Ok, InvalidInput, DivisionByZero, FileNotFound, NetworkDown };

[[nodiscard]] std::string_view to_string(ErrorCode ec) noexcept {
    switch (ec) {
        case ErrorCode::Ok:            return "Ok";
        case ErrorCode::InvalidInput:  return "InvalidInput";
        case ErrorCode::DivisionByZero:return "DivisionByZero";
        case ErrorCode::FileNotFound:  return "FileNotFound";
        case ErrorCode::NetworkDown:   return "NetworkDown";
    }
    return "Unknown";
}

struct ErrorResult {
    int       value{};
    ErrorCode code{ErrorCode::Ok};
    [[nodiscard]] bool ok() const noexcept { return code == ErrorCode::Ok; }
};

[[nodiscard]] ErrorResult divide_noexcept(int a, int b) noexcept {
    if (b == 0) return {0, ErrorCode::DivisionByZero};
    return {a / b, ErrorCode::Ok};
}

int main() {
    try { (void)divide(10, 0); }
    catch (const std::invalid_argument& e) { std::cout << std::format("Caught: {}\n", e.what()); }

    try { validate_number(-5); }
    catch (const ValidationError& e) { std::cout << std::format("Validation: {}\n", e.what()); }

    try { (void)safe_square(-4); }
    catch (const MyException& e) { std::cout << std::format("Custom: {}\n", e.what()); }

    try { open_file(""); }
    catch (const FileError& e) { std::cout << std::format("File: {}\n", e.what()); }

    try { wrapper_divide(5, 0); }
    catch (const std::exception& e) { std::cout << std::format("After rethrow: {}\n", e.what()); }

    try { throw std::runtime_error("Unexpected runtime error"); }
    catch (const std::exception& e) { std::cout << std::format("Standard: {}\n", e.what()); }
    catch (...) { std::cout << "Unknown exception\n"; }

    try { throw MathError("Demo error"); }
    catch (const MathError&    e) { std::cout << std::format("Specific MathError: {}\n", e.what()); }
    catch (const MyException&  e) { std::cout << std::format("Base MyException: {}\n",   e.what()); }

    std::cout << "\n--- try_divide ---\n";
    if (auto val = try_divide(10, 2))
        std::cout << std::format("try_divide(10,2): {}\n", *val);
    if (!try_divide(10, 0))
        std::cout << "try_divide(10,0): safely returned nullopt\n";

    std::cout << "\n--- Result<T,E> divide ---\n";
    {
        auto r   = divide_result(10, 2);
        auto bad = divide_result(10, 0);
        print_result(r,   "divide_result(10,2)");
        print_result(bad, "divide_result(10,0)");
    }

    std::cout << "\n--- Nested exceptions ---\n";
    try { nested_example(); }
    catch (const std::exception& e) { print_nested(e); }

    no_throw_function();

    std::cout << "\n--- Advanced ---\n";
    try { connect_server(false); }
    catch (const NetworkError& e) { std::cout << std::format("Network: {}\n", e.what()); }

    try { query_database(false); }
    catch (const DatabaseError& e) { std::cout << std::format("Database: {}\n", e.what()); }

    execute_safely([] { throw std::runtime_error("Lambda failure"); });
    execute_safely([] { validate_number(-100); });

    try { check_positive(-1); }
    catch (const ValidationError& e) { std::cout << std::format("Positive check: {}\n", e.what()); }

    ExceptionLogger logger;
    logger.log_exception([] { (void)divide(1, 0); });
    logger.log_exception([] { open_file(""); });

    std::cout << "\nStored logs:\n";
    logger.print_all();

    std::cout << "\n--- source_location diagnostics ---\n";
    try { throw ValidationError("demonstrating location tracking"); }
    catch (const MyException& e) { std::cout << e.diagnostic() << '\n'; }

    std::cout << "\n--- with_retry ---\n";
    int attempt = 0;
    with_retry(3, [&] {
        ++attempt;
        if (attempt < 3) throw std::runtime_error("transient failure");
        std::cout << std::format("Succeeded on attempt {}\n", attempt);
    }, "network call");

    std::cout << "\n--- ErrorCode (no-exception path) ---\n";
    {
        auto r1 = divide_noexcept(10, 2);
        auto r2 = divide_noexcept(10, 0);
        std::cout << std::format("divide_noexcept(10,2): value={} code={}\n",
                                 r1.value, to_string(r1.code));
        std::cout << std::format("divide_noexcept(10,0): value={} code={}\n",
                                 r2.value, to_string(r2.code));
        assert(r1.ok() && r1.value == 5);
        assert(!r2.ok() && r2.code == ErrorCode::DivisionByZero);
    }

    std::cout << "\n--- std::exception_ptr (cross-thread) ---\n";
    {
        std::exception_ptr eptr;
        std::thread t([&] {
            try {
                throw MathError("thrown from worker thread");
            } catch (...) {
                eptr = std::current_exception();
            }
        });
        t.join();
        if (eptr) {
            try {
                std::rethrow_exception(eptr);
            } catch (const MyException& e) {
                std::cout << std::format("Caught from thread: {}\n", e.what());
            }
        }
    }

    std::cout << "\n--- std::future exception transport ---\n";
    {
        auto fut = std::async(std::launch::async, [] {
            throw NetworkError("async task failed");
            return 0;
        });
        try {
            fut.get();
        } catch (const NetworkError& e) {
            std::cout << std::format("Future exception: {}\n", e.what());
        }
    }

    assert(try_divide(20, 5) == 4);
    assert(!try_divide(10, 0).has_value());
    assert(safe_square(5) == 25);
    assert(is_ok(divide_result(6, 3)));
    assert(!is_ok(divide_result(6, 0)));
    assert(logger.size() == 2);

    std::cout << "\nAll assertions passed.\n";
    return 0;
}
