// Exceptions and Custom Exceptions Exercise
// Exception hierarchy and handling

#include <iostream>
#include <exception>
#include <string>
#include <stdexcept>

class MyException : public std::exception {
private:
    std::string message;

public:
    MyException(const std::string& msg) : message(msg) {}

    const char* what() const noexcept override {
        return message.c_str();
    }
};

class ValidationError : public MyException {
public:
    ValidationError(const std::string& msg)
        : MyException("ValidationError: " + msg) {}
};

class MathError : public MyException {
public:
    MathError(const std::string& msg)
        : MyException("MathError: " + msg) {}
};

// 🔹 NEW: File-related exception
class FileError : public MyException {
public:
    FileError(const std::string& msg)
        : MyException("FileError: " + msg) {}
};

int divide(int a, int b) {
    if (b == 0) {
        throw std::invalid_argument("Division by zero");
    }
    return a / b;
}

void validate_number(int n) {
    if (n < 0) {
        throw ValidationError("Negative numbers are not allowed");
    }
}

int safe_square_root(int n) {
    if (n < 0) {
        throw MathError("Square root of negative number");
    }
    return n * n; // simple demo
}

// 🔹 NEW: function demonstrating another exception type
void open_file(const std::string& filename) {
    if (filename.empty()) {
        throw FileError("Filename is empty");
    }
    // pretend file opened
    std::cout << "File opened: " << filename << "\n";
}

// 🔹 NEW: wrapper with rethrow
void wrapper_divide(int a, int b) {
    try {
        std::cout << "Wrapper result: " << divide(a, b) << "\n";
    } catch (...) {
        std::cout << "Exception in wrapper, rethrowing...\n";
        throw;  // rethrow original exception
    }
}

// ---------------- EXTRA ADDITIONS ----------------

// 🔹 NEW: safe divide returning optional-like behavior
bool try_divide(int a, int b, int& result) noexcept {
    if (b == 0) return false;
    result = a / b;
    return true;
}

// 🔹 NEW: nested exception demo
void nested_example() {
    try {
        throw MathError("Inner math failure");
    } catch (...) {
        std::throw_with_nested(MyException("Outer exception with nested cause"));
    }
}

// 🔹 NEW: function to print nested exceptions
void print_nested(const std::exception& e, int level = 0) {
    std::cout << std::string(level * 2, ' ')
              << "Exception: " << e.what() << "\n";
    try {
        std::rethrow_if_nested(e);
    } catch (const std::exception& nested) {
        print_nested(nested, level + 1);
    } catch (...) {}
}

// 🔹 NEW: noexcept demonstration
void no_throw_function() noexcept {
    std::cout << "This function guarantees no exceptions\n";
}

// ------------------------------------------------

int main() {

    // Example 1: standard exception
    try {
        divide(10, 0);
    } catch (const std::invalid_argument& e) {
        std::cout << "Caught: " << e.what() << "\n";
    }

    // Example 2: custom validation exception
    try {
        validate_number(-5);
    } catch (const ValidationError& e) {
        std::cout << "Validation exception: " << e.what() << "\n";
    }

    // Example 3: custom math exception
    try {
        safe_square_root(-4);
    } catch (const MyException& e) {
        std::cout << "Custom exception: " << e.what() << "\n";
    }

    // 🔹 NEW: file exception demo
    try {
        open_file("");
    } catch (const FileError& e) {
        std::cout << "File exception: " << e.what() << "\n";
    }

    // 🔹 NEW: rethrow demo
    try {
        wrapper_divide(5, 0);
    } catch (const std::exception& e) {
        std::cout << "Caught after rethrow: " << e.what() << "\n";
    }

    // Catch-all example
    try {
        throw std::runtime_error("Unexpected runtime error");
    } catch (const std::exception& e) {
        std::cout << "Standard exception: " << e.what() << "\n";
    } catch (...) {
        std::cout << "Unknown exception caught\n";
    }

    // 🔹 NEW: multiple catch order demo
    try {
        throw MathError("Demo error");
    } catch (const MathError& e) {
        std::cout << "Caught specific MathError: " << e.what() << "\n";
    } catch (const MyException& e) {
        std::cout << "Caught base MyException: " << e.what() << "\n";
    }

    // ---------------- EXTRA USAGE ----------------

    std::cout << "\n--- Extra Tests ---\n";

    // try_divide usage
    int result;
    if (try_divide(10, 2, result)) {
        std::cout << "try_divide success: " << result << "\n";
    } else {
        std::cout << "try_divide failed\n";
    }

    if (!try_divide(10, 0, result)) {
        std::cout << "try_divide handled division by zero safely\n";
    }

    // nested exception demo
    try {
        nested_example();
    } catch (const std::exception& e) {
        std::cout << "Nested exception trace:\n";
        print_nested(e);
    }

    // noexcept demo
    no_throw_function();

    // ------------------------------------------------

    return 0;
}
