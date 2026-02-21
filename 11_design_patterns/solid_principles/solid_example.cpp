// SOLID Principles Exercise
// Single Responsibility, Open/Closed, Liskov, Interface Segregation, Dependency Inversion

#include <iostream>
#include <memory>

// Single Responsibility: Each class has one job
class Logger {
public:
    virtual ~Logger() = default;
    virtual void log(const std::string& msg) = 0;
};

class ConsoleLogger : public Logger {
public:
    void log(const std::string& msg) override {
        std::cout << msg << "\n";
    }
};

// Dependency Inversion
class Application {
private:
    std::shared_ptr<Logger> logger;
    
public:
    Application(std::shared_ptr<Logger> log) : logger(log) {}
    
    void run() {
        logger->log("Application started");
    }
};

int main() {
    auto console_logger = std::make_shared<ConsoleLogger>();
    Application app(console_logger);
    app.run();
    
    return 0;
}
