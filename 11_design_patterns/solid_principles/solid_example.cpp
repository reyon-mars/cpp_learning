// SOLID Principles Exercise
// Single Responsibility, Open/Closed, Liskov, Interface Segregation, Dependency Inversion

#include <iostream>
#include <memory>

// ---------------- Single Responsibility ----------------

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

// ---------------- Open / Closed Principle ----------------

class FileLogger : public Logger {
public:
    void log(const std::string& msg) override {
        std::cout << "[File] " << msg << "\n";
    }
};

// ---------------- Interface Segregation ----------------

class Printer {
public:
    virtual ~Printer() = default;
    virtual void print(const std::string& doc) = 0;
};

class Scanner {
public:
    virtual ~Scanner() = default;
    virtual void scan() = 0;
};

class MultiFunctionPrinter : public Printer, public Scanner {
public:
    void print(const std::string& doc) override {
        std::cout << "Printing: " << doc << "\n";
    }

    void scan() override {
        std::cout << "Scanning document\n";
    }
};

// ---------------- Liskov Substitution ----------------

class Shape {
public:
    virtual ~Shape() = default;
    virtual int area() const = 0;
};

class Rectangle : public Shape {
protected:
    int width;
    int height;

public:
    Rectangle(int w, int h) : width(w), height(h) {}

    int area() const override {
        return width * height;
    }
};

class Square : public Shape {
private:
    int side;

public:
    Square(int s) : side(s) {}

    int area() const override {
        return side * side;
    }
};

// ---------------- Dependency Inversion ----------------

class Application {
private:
    std::shared_ptr<Logger> logger;
    
public:
    Application(std::shared_ptr<Logger> log) : logger(log) {}
    
    void run() {
        logger->log("Application started");
    }
};

// ---------------- Main ----------------

int main() {

    // Dependency Inversion
    auto console_logger = std::make_shared<ConsoleLogger>();
    Application app(console_logger);
    app.run();

    std::cout << "\n";

    // Open/Closed (switch logger implementation)
    auto file_logger = std::make_shared<FileLogger>();
    Application app2(file_logger);
    app2.run();

    std::cout << "\n";

    // Interface Segregation
    MultiFunctionPrinter printer;
    printer.print("Report.pdf");
    printer.scan();

    std::cout << "\n";

    // Liskov Substitution
    std::unique_ptr<Shape> shape1 = std::make_unique<Rectangle>(4, 5);
    std::unique_ptr<Shape> shape2 = std::make_unique<Square>(4);

    std::cout << "Rectangle area: " << shape1->area() << "\n";
    std::cout << "Square area: " << shape2->area() << "\n";

    return 0;
}
