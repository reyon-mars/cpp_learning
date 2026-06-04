#include <iostream>
#include <memory>
#include <vector>
#include <typeinfo>
#include <algorithm>
#include <numeric>
#include <cassert>
#include <numbers>

class Shape {
public:
    Shape()          { std::cout << "Shape constructed\n"; }
    virtual ~Shape() { std::cout << "Shape destroyed\n";   }

    virtual void draw()  const = 0;
    virtual void info()  const = 0;

    [[nodiscard]] virtual double                  area()  const = 0;
    [[nodiscard]] virtual const char*             name()  const = 0;
    [[nodiscard]] virtual std::unique_ptr<Shape>  clone() const = 0;
};

class Circle final : public Shape {
public:
    explicit Circle(double r) : radius_{r} {
        std::cout << "Circle constructed\n";
    }
    ~Circle() override { std::cout << "Circle destroyed\n"; }

    void draw() const override { std::cout << "Drawing Circle\n"; }
    void info() const override { std::cout << "Circle radius=" << radius_ << "\n"; }

    [[nodiscard]] double area()  const override { return std::numbers::pi * radius_ * radius_; }
    [[nodiscard]] const char* name() const override { return "Circle"; }
    [[nodiscard]] std::unique_ptr<Shape> clone() const override {
        return std::make_unique<Circle>(*this);
    }

    [[nodiscard]] double getRadius() const noexcept { return radius_; }

private:
    double radius_;
};

class Rectangle final : public Shape {
public:
    Rectangle(double w, double h) : width_{w}, height_{h} {
        std::cout << "Rectangle constructed\n";
    }
    ~Rectangle() override { std::cout << "Rectangle destroyed\n"; }

    void draw() const override { std::cout << "Drawing Rectangle\n"; }
    void info() const override {
        std::cout << "Rectangle " << width_ << "x" << height_ << "\n";
    }

    [[nodiscard]] double area()  const override { return width_ * height_; }
    [[nodiscard]] const char* name() const override { return "Rectangle"; }
    [[nodiscard]] std::unique_ptr<Shape> clone() const override {
        return std::make_unique<Rectangle>(*this);
    }

private:
    double width_, height_;
};

class Triangle final : public Shape {
public:
    Triangle(double b, double h) : base_{b}, height_{h} {
        std::cout << "Triangle constructed\n";
    }
    ~Triangle() override { std::cout << "Triangle destroyed\n"; }

    void draw() const override { std::cout << "Drawing Triangle\n"; }
    void info() const override {
        std::cout << "Triangle base=" << base_ << " height=" << height_ << "\n";
    }

    [[nodiscard]] double area()  const override { return 0.5 * base_ * height_; }
    [[nodiscard]] const char* name() const override { return "Triangle"; }
    [[nodiscard]] std::unique_ptr<Shape> clone() const override {
        return std::make_unique<Triangle>(*this);
    }

private:
    double base_, height_;
};

using ShapeVec = std::vector<std::unique_ptr<Shape>>;

[[nodiscard]] double totalArea(const ShapeVec& shapes) {
    return std::accumulate(shapes.begin(), shapes.end(), 0.0,
        [](double sum, const auto& s) { return sum + s->area(); });
}

[[nodiscard]] const Shape* largestShape(const ShapeVec& shapes) {
    if (shapes.empty()) return nullptr;
    return std::ranges::max_element(shapes, {},
        [](const auto& s) { return s->area(); })->get();
}

[[nodiscard]] long countCircles(const ShapeVec& shapes) {
    return std::ranges::count_if(shapes,
        [](const auto& s) { return dynamic_cast<const Circle*>(s.get()) != nullptr; });
}

[[nodiscard]] ShapeVec cloneAll(const ShapeVec& shapes) {
    ShapeVec copies;
    copies.reserve(shapes.size());
    std::ranges::transform(shapes, std::back_inserter(copies),
        [](const auto& s) { return s->clone(); });
    return copies;
}

[[nodiscard]] std::unique_ptr<Shape> createShape(int type) {
    switch (type) {
        case 1:  return std::make_unique<Circle>(3.0);
        case 2:  return std::make_unique<Rectangle>(2.0, 5.0);
        default: return std::make_unique<Triangle>(4.0, 6.0);
    }
}

void processShape(const Shape& s) {
    std::cout << "[Processing] " << s.name() << "\n";
    s.draw();
    std::cout << "Area=" << s.area() << "\n";
}

void printType(const Shape& s) {
    std::cout << "RTTI: " << typeid(s).name() << "\n";
}

int main() {
    ShapeVec shapes;
    shapes.reserve(3);
    shapes.push_back(std::make_unique<Circle>(5.0));
    shapes.push_back(std::make_unique<Rectangle>(4.0, 6.0));
    shapes.push_back(std::make_unique<Triangle>(3.0, 8.0));
    assert(!shapes.empty());

    std::cout << "\n--- Polymorphic draw + area ---\n";
    for (const auto& s : shapes) {
        s->draw();
        std::cout << "Area=" << s->area() << "\n";
    }

    std::cout << "\n--- Reference / raw pointer ---\n";
    shapes[0]->draw();
    shapes[1]->draw();

    std::cout << "\n--- dynamic_cast ---\n";
    if (const auto* c = dynamic_cast<const Circle*>(shapes[0].get())) {
        std::cout << "shapes[0] is Circle, radius=" << c->getRadius() << "\n";
    }

    std::cout << "\n--- Shape names ---\n";
    for (const auto& s : shapes) { std::cout << s->name() << "\n"; }

    std::cout << "\n--- Total area ---\n";
    std::cout << "Total=" << totalArea(shapes) << "\n";

    std::cout << "\n--- RTTI ---\n";
    for (const auto& s : shapes) { printType(*s); }

    std::cout << "\n--- Clone single ---\n";
    auto cloned = shapes[0]->clone();
    cloned->draw();

    std::cout << "\n--- Factory ---\n";
    createShape(2)->draw();

    std::cout << "\n--- processShape ---\n";
    processShape(*shapes[2]);

    std::cout << "\n--- Largest shape ---\n";
    if (const Shape* lg = largestShape(shapes)) {
        std::cout << "Largest=" << lg->name() << " area=" << lg->area() << "\n";
    }

    std::cout << "\n--- Shape info ---\n";
    for (const auto& s : shapes) { s->info(); }

    std::cout << "\n--- Circle count ---\n";
    std::cout << "Circles=" << countCircles(shapes) << "\n";

    std::cout << "\n--- Clone all ---\n";
    for (const auto& s : cloneAll(shapes)) {
        std::cout << "Cloned " << s->name() << " area=" << s->area() << "\n";
    }

    std::cout << "\n--- End of main ---\n";
    return 0;
}
