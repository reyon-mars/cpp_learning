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

// Helper printer
void print_result(const Result& r) {
    if (r.is_ok()) {
        std::cout << "Result: " << r.get_value() << "\n";
    } else {
        std::cout << "Error: " << to_string(r.get_error()) << "\n";
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

    return 0;
}
