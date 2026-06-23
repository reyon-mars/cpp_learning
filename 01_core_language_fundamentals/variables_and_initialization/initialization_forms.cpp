#include <iostream>
#include <initializer_list>
#include <vector>
#include <array>
#include <string>
#include <string_view>
#include <span>
#include <ranges>
#include <type_traits>
#include <cassert>

struct Point {
    int x = 0;
    int y = 0;
};

static_assert(std::is_aggregate_v<Point>, "Point must be an aggregate");

void print_point(const Point& p) {
    std::cout << '(' << p.x << ", " << p.y << ")\n";
}

struct Student {
    std::string name;
    int         age = 0;
};

void print_student(const Student& s) {
    std::cout << "Student: " << s.name << ", age " << s.age << '\n';
}

struct Config {
    int  version = 0;
    bool debug   = false;
};

struct Settings {
    int width  = 800;
    int height = 600;
};

class ExplicitDemo {
    int value_;
public:
    explicit ExplicitDemo(int v) noexcept : value_(v) {}
    [[nodiscard]] int value() const noexcept { return value_; }
};

class Box {
    int width_;
    int height_;
public:
    Box(int w, int h) noexcept : width_(w), height_(h) {}
    void show() const {
        std::cout << "Box(" << width_ << ", " << height_ << ")\n";
    }
};

class Rectangle {
    int width_;
    int height_;
public:
    Rectangle() noexcept : Rectangle(1, 1) {}
    Rectangle(int w, int h) noexcept : width_(w), height_(h) {}
    void print() const {
        std::cout << "Rectangle(" << width_ << ", " << height_ << ")\n";
    }
};

class NumberList {
    std::vector<int> values_;
public:
    NumberList(std::initializer_list<int> vals) : values_(vals) {}
    void print() const {
        for (int v : values_) std::cout << v << ' ';
        std::cout << '\n';
    }
};

void show_init_difference() {
    const int x(5);
    const int y{5};
    std::cout << "direct(" << x << ")  uniform{" << y << "}\n";
}

void value_init_demo() {
    const int    x{};
    const double y{};
    const bool   z{};
    std::cout << "\nValue initialization:\n"
              << "int:    " << x << '\n'
              << "double: " << y << '\n'
              << "bool:   " << z << '\n';
}

void narrowing_demo() {
    const int a = static_cast<int>(3.14);
    std::cout << "Explicit cast (int)3.14 = " << a << '\n';
    std::cout << "Brace init int{3.14} would be a compile error\n";
}

int main() {
    std::cout << "\nAdvanced Initialization Concepts:\n";

    const Point p4{};
    std::cout << "p4 (zero-initialized) = ";
    print_point(p4);

    const ExplicitDemo ex1(10);
    std::cout << "ExplicitDemo value: " << ex1.value() << "\n";

    const auto list = {1, 2, 3};
    static_assert(std::is_same_v<decltype(list), const std::initializer_list<int>>,
                  "list must be std::initializer_list<int>");
    std::cout << "Initializer list size: " << list.size() << '\n';

    show_init_difference();

    const Student s1{"Alice", 20};
    print_student(s1);

    const Box b1(5, 10);
    b1.show();

    narrowing_demo();

    const NumberList nums{1, 2, 3, 4, 5};
    std::cout << "NumberList: ";
    nums.print();

    constexpr Config cfg{2, true};
    std::cout << "Config version: " << cfg.version << '\n';
    std::cout << "Debug mode: " << (cfg.debug ? "ON" : "OFF") << '\n';

    assert(ex1.value() == 10);

    const std::vector<Point> points = {{1, 2}, {3, 4}, {5, 6}};
    std::cout << "\nVector of points:\n";
    for (const auto& p : points) print_point(p);

    const Settings settings{};
    std::cout << "\nSettings: width=" << settings.width
              << " height=" << settings.height << '\n';

    const Rectangle r1;
    const Rectangle r2(10, 20);
    std::cout << "\nDelegating constructors:\n";
    r1.print();
    r2.print();

    value_init_demo();

    constexpr std::array<int, 5> arr{1, 2, 3, 4, 5};
    std::cout << "\nstd::array: ";
    for (int n : arr) std::cout << n << ' ';
    std::cout << '\n';

    const Point origin{};
    std::cout << "\nOrigin: ";
    print_point(origin);

    static_assert(cfg.version == 2);
    static_assert(cfg.debug   == true);

    std::cout << "\nAll assertions passed.\n";
    return 0;
}
