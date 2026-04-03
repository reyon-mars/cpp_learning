// SOLID Principles Exercise
// Single Responsibility, Open/Closed, Liskov, Interface Segregation, Dependency Inversion

#include <iostream>
#include <memory>
#include <ctime>   // ✅ ADDED

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

// ✅ ADDED: Separate responsibility (writing logic)
class FileWriter {
public:
    void write(const std::string& msg) {
        std::cout << "[Writing to file]: " << msg << "\n";
    }
};

// ---------------- Open / Closed Principle ----------------

class FileLogger : public Logger {
public:
    void log(const std::string& msg) override {
        std::cout << "[File] " << msg << "\n";
    }
};

// ✅ ADDED: Extend behavior without modifying existing classes
class TimestampLogger : public Logger {
public:
    void log(const std::string& msg) override {
        std::time_t now = std::time(nullptr);
        std::cout << "[Time: " << now << "] " << msg << "\n";
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

// ✅ ADDED: Class that only needs printing
class SimplePrinter : public Printer {
public:
    void print(const std::string& doc) override {
        std::cout << "Simple printing: " << doc << "\n";
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

// ✅ ADDED: Another valid Shape
class Circle : public Shape {
private:
    int radius;

public:
    Circle(int r) : radius(r) {}

    int area() const override {
        return 3.14 * radius * radius;
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

    // ✅ ADDED: Change dependency at runtime
    void set_logger(std::shared_ptr<Logger> new_logger) {
        logger = new_logger;
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

    // ✅ ADDED: Timestamp logger
    auto time_logger = std::make_shared<TimestampLogger>();
    app2.set_logger(time_logger);
    app2.run();

    std::cout << "\n";

    // Interface Segregation
    MultiFunctionPrinter printer;
    printer.print("Report.pdf");
    printer.scan();

    // ✅ ADDED: Simple printer usage
    SimplePrinter simple;
    simple.print("Notes.txt");

    std::cout << "\n";

    // Liskov Substitution
    std::unique_ptr<Shape> shape1 = std::make_unique<Rectangle>(4, 5);
    std::unique_ptr<Shape> shape2 = std::make_unique<Square>(4);
    std::unique_ptr<Shape> shape3 = std::make_unique<Circle>(3); // ✅ ADDED

    std::cout << "Rectangle area: " << shape1->area() << "\n";
    std::cout << "Square area: " << shape2->area() << "\n";
    std::cout << "Circle area: " << shape3->area() << "\n"; // ✅ ADDED

    return 0;
}
