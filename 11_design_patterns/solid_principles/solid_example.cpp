#include <iostream>
#include <memory>
#include <ctime>
#include <vector>
#include <string>
#include <string_view>
#include <algorithm>
#include <numeric>
#include <ranges>
#include <format>
#include <chrono>
#include <span>

class Logger {
public:
    virtual ~Logger()                        = default;
    virtual void log(std::string_view msg)   = 0;
};

class ConsoleLogger final : public Logger {
public:
    void log(std::string_view msg) override {
        std::cout << msg << '\n';
    }
};

class FileWriter {
public:
    void write(std::string_view msg) const {
        std::cout << std::format("[File] {}\n", msg);
    }
};

class FileLogger final : public Logger {
public:
    void log(std::string_view msg) override {
        writer_.write(msg);
    }
private:
    FileWriter writer_;
};

class TimestampLogger final : public Logger {
public:
    void log(std::string_view msg) override {
        const auto now = std::chrono::system_clock::now();
        const auto ts  = std::chrono::system_clock::to_time_t(now);
        std::cout << std::format("[ts={}] {}\n", ts, msg);
    }
};

class Printer {
public:
    virtual ~Printer()                           = default;
    virtual void print(std::string_view doc)     = 0;
};

class Scanner {
public:
    virtual ~Scanner()  = default;
    virtual void scan() = 0;
};

class MultiFunctionPrinter final : public Printer, public Scanner {
public:
    void print(std::string_view doc) override {
        std::cout << std::format("Printing: {}\n", doc);
    }
    void scan() override {
        std::cout << "Scanning document\n";
    }
};

class SimplePrinter final : public Printer {
public:
    void print(std::string_view doc) override {
        std::cout << std::format("Simple print: {}\n", doc);
    }
};

class Shape {
public:
    virtual ~Shape()                  = default;
    [[nodiscard]] virtual int area()  const noexcept = 0;
    [[nodiscard]] virtual std::string_view name() const noexcept = 0;
};

class Rectangle final : public Shape {
    int width_, height_;
public:
    Rectangle(int w, int h) noexcept : width_{w}, height_{h} {}
    [[nodiscard]] int area()            const noexcept override { return width_ * height_; }
    [[nodiscard]] std::string_view name() const noexcept override { return "Rectangle"; }
};

class Square final : public Shape {
    int side_;
public:
    explicit Square(int s) noexcept : side_{s} {}
    [[nodiscard]] int area()            const noexcept override { return side_ * side_; }
    [[nodiscard]] std::string_view name() const noexcept override { return "Square"; }
};

class Circle final : public Shape {
    int radius_;
    static constexpr double kPi{3.14159265358979};
public:
    explicit Circle(int r) noexcept : radius_{r} {}
    [[nodiscard]] int area()            const noexcept override {
        return static_cast<int>(kPi * radius_ * radius_);
    }
    [[nodiscard]] std::string_view name() const noexcept override { return "Circle"; }
};

class Application {
    std::shared_ptr<Logger> logger_;
public:
    explicit Application(std::shared_ptr<Logger> logger)
        : logger_{std::move(logger)} {}

    void set_logger(std::shared_ptr<Logger> logger) {
        logger_ = std::move(logger);
    }

    [[nodiscard]] bool has_logger()  const noexcept { return logger_ != nullptr; }

    void run()  { logger_->log("Application started"); }
    void stop() { logger_->log("Application stopped"); }
};

using ShapeVec = std::vector<std::unique_ptr<Shape>>;

void print_shapes(std::span<const std::unique_ptr<Shape>> shapes) {
    for (const auto& s : shapes)
        std::cout << std::format("  {}: area={}\n", s->name(), s->area());
}

[[nodiscard]] int total_area(std::span<const std::unique_ptr<Shape>> shapes) noexcept {
    return std::transform_reduce(
        shapes.begin(), shapes.end(),
        0, std::plus<>{},
        [](const auto& s){ return s->area(); });
}

void divider() { std::cout << "-----------------------------\n"; }

void section(std::string_view title) {
    std::cout << std::format("\n=== {} ===\n", title);
}

int main() {
    auto console = std::make_shared<ConsoleLogger>();
    Application app{console};
    app.run();
    std::cout << std::format("Has logger? {}\n", app.has_logger() ? "Yes" : "No");

    divider();

    auto file_log = std::make_shared<FileLogger>();
    Application app2{file_log};
    app2.run();

    divider();

    app2.set_logger(std::make_shared<TimestampLogger>());
    app2.run();

    divider();

    MultiFunctionPrinter mfp;
    mfp.print("Report.pdf");
    mfp.scan();

    SimplePrinter sp;
    sp.print("Notes.txt");

    divider();

    section("Liskov Substitution");
    const std::array<std::unique_ptr<Shape>, 3> singles{
        std::make_unique<Rectangle>(4, 5),
        std::make_unique<Square>(4),
        std::make_unique<Circle>(3),
    };
    for (const auto& s : singles)
        std::cout << std::format("{} area: {}\n", s->name(), s->area());

    divider();

    section("Shape Collection");
    ShapeVec shapes;
    shapes.push_back(std::make_unique<Rectangle>(2, 3));
    shapes.push_back(std::make_unique<Square>(5));
    shapes.push_back(std::make_unique<Circle>(2));

    print_shapes(shapes);

    std::cout << std::format("Count:      {}\n", shapes.size());
    std::cout << std::format("Total area: {}\n", total_area(shapes));

    const auto largest = std::ranges::max_element(
        shapes, {}, [](const auto& s){ return s->area(); });
    std::cout << std::format("Largest:    {} (area={})\n",
                             (*largest)->name(), (*largest)->area());

    divider();

    section("Logger Polymorphism");
    for (auto& lg : {
        std::static_pointer_cast<Logger>(console),
        std::static_pointer_cast<Logger>(file_log),
    }) {
        lg->log("Test message");
    }

    section("Numeric Utility");
    constexpr std::array nums{10, 20, 30, 40};
    const int sum = std::reduce(nums.begin(), nums.end(), 0);
    std::cout << std::format("Sum:     {}\n", sum);
    std::cout << std::format("Average: {:.1f}\n",
                             static_cast<double>(sum) / nums.size());
    std::cout << std::format("Max:     {}\n",
                             *std::ranges::max_element(nums));

    app.stop();

    return 0;
}
