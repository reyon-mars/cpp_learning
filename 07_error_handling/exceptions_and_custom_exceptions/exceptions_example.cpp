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

class MyException : public std::exception {
    std::string message_;
public:
    explicit MyException(std::string msg) : message_(std::move(msg)) {}
    [[nodiscard]] const char* what() const noexcept override { return message_.c_str(); }
};

class ValidationError : public MyException {
public:
    explicit ValidationError(std::string_view msg)
        : MyException("ValidationError: " + std::string(msg)) {}
};

class MathError : public MyException {
public:
    explicit MathError(std::string_view msg)
        : MyException("MathError: " + std::string(msg)) {}
};

class FileError : public MyException {
public:
    explicit FileError(std::string_view msg)
        : MyException("FileError: " + std::string(msg)) {}
};

class NetworkError : public MyException {
public:
    explicit NetworkError(std::string_view msg)
        : MyException("NetworkError: " + std::string(msg)) {}
};

class DatabaseError : public MyException {
public:
    explicit DatabaseError(std::string_view msg)
        : MyException("DatabaseError: " + std::string(msg)) {}
};

[[nodiscard]] int divide(int a, int b) {
    if (b == 0) throw std::invalid_argument("Division by zero");
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
    std::cout << "File opened: " << filename << '\n';
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
        std::cout << "Wrapper result: " << divide(a, b) << '\n';
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
    std::cout << std::string(static_cast<std::size_t>(level) * 2, ' ')
              << "Exception: " << e.what() << '\n';
    try {
        std::rethrow_if_nested(e);
    } catch (const std::exception& nested) {
        print_nested(nested, level + 1);
    } catch (...) {}
}

void no_throw_function() noexcept {
    std::cout << "Guaranteed no exceptions\n";
}

void execute_safely(std::invocable auto fn) {
    try {
        std::invoke(fn);
    } catch (const std::exception& e) {
        std::cout << "Handled in execute_safely: " << e.what() << '\n';
    }
}

int main() {
    try {
        (void)divide(10, 0);
    } catch (const std::invalid_argument& e) {
        std::cout << "Caught: " << e.what() << '\n';
    }

    try {
        validate_number(-5);
    } catch (const ValidationError& e) {
        std::cout << "Validation: " << e.what() << '\n';
    }

    try {
        (void)safe_square(-4);
    } catch (const MyException& e) {
        std::cout << "Custom: " << e.what() << '\n';
    }

    try {
        open_file("");
    } catch (const FileError& e) {
        std::cout << "File: " << e.what() << '\n';
    }

    try {
        wrapper_divide(5, 0);
    } catch (const std::exception& e) {
        std::cout << "After rethrow: " << e.what() << '\n';
    }

    try {
        throw std::runtime_error("Unexpected runtime error");
    } catch (const std::exception& e) {
        std::cout << "Standard: " << e.what() << '\n';
    } catch (...) {
        std::cout << "Unknown exception\n";
    }

    try {
        throw MathError("Demo error");
    } catch (const MathError& e) {
        std::cout << "Specific MathError: " << e.what() << '\n';
    } catch (const MyException& e) {
        std::cout << "Base MyException: " << e.what() << '\n';
    }

    std::cout << "\n--- try_divide ---\n";
    if (auto val = try_divide(10, 2))
        std::cout << "try_divide(10,2): " << *val << '\n';

    if (!try_divide(10, 0))
        std::cout << "try_divide(10,0): safely returned nullopt\n";

    std::cout << "\n--- Nested exceptions ---\n";
    try {
        nested_example();
    } catch (const std::exception& e) {
        print_nested(e);
    }

    no_throw_function();

    std::cout << "\n--- Advanced ---\n";
    try {
        connect_server(false);
    } catch (const NetworkError& e) {
        std::cout << "Network: " << e.what() << '\n';
    }

    try {
        query_database(false);
    } catch (const DatabaseError& e) {
        std::cout << "Database: " << e.what() << '\n';
    }

    execute_safely([] { throw std::runtime_error("Lambda failure"); });
    execute_safely([] { validate_number(-100); });

    try {
        check_positive(-1);
    } catch (const ValidationError& e) {
        std::cout << "Positive check: " << e.what() << '\n';
    }

    std::vector<std::string> logs;
    auto log_exception = [&](auto fn) {
        try { fn(); } catch (const std::exception& e) { logs.emplace_back(e.what()); }
    };

    log_exception([] { (void)divide(1, 0); });
    log_exception([] { open_file(""); });

    std::cout << "\nStored logs:\n";
    for (const auto& log : logs) std::cout << "- " << log << '\n';

    assert(try_divide(20, 5) == 4);
    assert(!try_divide(10, 0).has_value());
    assert(safe_square(5) == 25);

    std::cout << "\nAll assertions passed.\n";
    return 0;
}
