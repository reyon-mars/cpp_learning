// C++23 and Beyond Exercise
// std::expected and future features

#include <iostream>

// Simple Expected-like implementation for demonstration
template<typename T, typename E>
class Result {
private:
    bool is_success;
    T value;
    E error;
    
public:
    Result(const T& v) : is_success(true), value(v), error() {}
    Result(const E& e) : is_success(false), value(), error(e) {}
    
    bool success() const { return is_success; }
    const T& get_value() const { return value; }
    const E& get_error() const { return error; }
};

int main() {
    Result<int, std::string> success(42);
    Result<int, std::string> failure("Error occurred");
    
    if (success.success()) {
        std::cout << "Success: " << success.get_value() << "\n";
    }
    
    if (!failure.success()) {
        std::cout << "Failure: " << failure.get_error() << "\n";
    }
    
    return 0;
}
