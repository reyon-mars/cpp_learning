// Exceptions and Custom Exceptions Exercise
// Exception hierarchy and handling

#include <iostream>
#include <exception>

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
    ValidationError(const std::string& msg) : MyException("ValidationError: " + msg) {}
};

int divide(int a, int b) {
    if (b == 0) {
        throw std::invalid_argument("Division by zero");
    }
    return a / b;
}

int main() {
    try {
        divide(10, 0);
    } catch (const std::invalid_argument& e) {
        std::cout << "Caught: " << e.what() << "\n";
    } catch (const MyException& e) {
        std::cout << "Custom exception: " << e.what() << "\n";
    } catch (const std::exception& e) {
        std::cout << "Standard exception: " << e.what() << "\n";
    }
    
    return 0;
}
