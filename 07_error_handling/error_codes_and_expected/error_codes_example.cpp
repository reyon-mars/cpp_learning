// Error Codes and Expected Exercise
// Alternative error handling patterns

#include <iostream>

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

Result divide(int a, int b) {
    if (b == 0) {
        return Result(ErrorCode::DivisionByZero);
    }
    return Result(a / b);
}

int main() {
    auto result = divide(10, 2);
    if (result.is_ok()) {
        std::cout << "Result: " << result.get_value() << "\n";
    } else {
        std::cout << "Error code: " << static_cast<int>(result.get_error()) << "\n";
    }
    
    auto bad_result = divide(10, 0);
    if (!bad_result.is_ok()) {
        std::cout << "Division by zero error\n";
    }
    
    return 0;
}
