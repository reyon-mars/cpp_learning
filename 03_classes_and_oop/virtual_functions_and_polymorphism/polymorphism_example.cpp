#include <iostream>
#include <memory>
#include <vector>
#include <typeinfo>

class Shape {
public:
    Shape() { std::cout << "Shape constructed\n"; }
    virtual ~Shape() { std::cout << "Shape destroyed\n"; }

    virtual void draw() const {
        std::cout << "Drawing shape\n";
    }

    virtual double area() const = 0;

    virtual const char* name() const {
        return "Shape";
    }

    // ✅ ADDED: clone pattern
    virtual std::unique_ptr<Shape> clone() const = 0;
};

class Circle : public Shape {
private:
    double radius;

public:
    Circle(double r) : radius(r) {
        std::cout << "Circle constructed\n";
    }

    ~Circle() override {
        std::cout << "Circle destroyed\n";
    }

    void draw() const override {
        std::cout << "Drawing circle\n";
    }

    double area() const override {
        return 3.14159 * radius * radius;
    }

    const char* name() const override {
        return "Circle";
    }

    // ✅ ADDED clone
    std::unique_ptr<Shape> clone() const override {
        return std::make_unique<Circle>(*this);
    }
};

class Rectangle : public Shape {
private:
    double width, height;

public:
    Rectangle(double w, double h) : width(w), height(h) {
        std::cout << "Rectangle constructed\n";
    }

    ~Rectangle() override {
        std::cout << "Rectangle destroyed\n";
    }

    void draw() const override {
        std::cout << "Drawing rectangle\n";
    }

    double area() const override {
        return width * height;
    }

    const char* name() const override {
        return "Rectangle";
    }

    std::unique_ptr<Shape> clone() const override {
        return std::make_unique<Rectangle>(*this);
    }
};

// ✅ ADDED: new derived class
class Triangle final : public Shape {
private:
    double base, height;

public:
    Triangle(double b, double h) : base(b), height(h) {
        std::cout << "Triangle constructed\n";
    }

    ~Triangle() override {
        std::cout << "Triangle destroyed\n";
    }

    void draw() const override final { // final prevents further override
        std::cout << "Drawing triangle\n";
    }

    double area() const override {
        return 0.5 * base * height;
    }

    const char* name() const override {
        return "Triangle";
    }

    std::unique_ptr<Shape> clone() const override {
        return std::make_unique<Triangle>(*this);
    }
};

// ----------- EXISTING ADDITIONS -----------

double totalArea(const std::vector<std::unique_ptr<Shape>>& shapes) {
    double sum = 0;
    for (const auto& s : shapes)
        sum += s->area();
    return sum;
}

void printType(const Shape* s) {
    std::cout << "RTTI type: " << typeid(*s).name() << "\n";
}

// ----------- NEW ADDITIONS -----------

// ✅ Smart factory
std::unique_ptr<Shape> createShape(int type) {
    if (type == 1) return std::make_unique<Circle>(3.0);
    if (type == 2) return std::make_unique<Rectangle>(2.0, 5.0);
    return std::make_unique<Triangle>(4.0, 6.0);
}

// ✅ Virtual dispatch helper
void processShape(const Shape& s) {
    std::cout << "[Processing] " << s.name() << "\n";
    s.draw();
    std::cout << "Area: " << s.area() << "\n";
}

// ------------------------------------

int main() {

    std::vector<std::unique_ptr<Shape>> shapes;

    shapes.push_back(std::make_unique<Circle>(5.0));
    shapes.push_back(std::make_unique<Rectangle>(4.0, 6.0));
    shapes.push_back(std::make_unique<Triangle>(3.0, 8.0)); // ✅ ADDED

    std::cout << "\n--- Polymorphic Calls ---\n";

    for (const auto& shape : shapes) {
        shape->draw();
        std::cout << "Area: " << shape->area() << "\n";
    }

    std::cout << "\n--- Base Reference Example ---\n";
    Shape& ref = *shapes[0];
    ref.draw();

    std::cout << "\n--- Raw Pointer Polymorphism ---\n";
    Shape* rawPtr = shapes[1].get();
    rawPtr->draw();
    std::cout << "Area: " << rawPtr->area() << "\n";

    std::cout << "\n--- Dynamic Cast Check ---\n";
    if (Circle* c = dynamic_cast<Circle*>(shapes[0].get())) {
        std::cout << "Confirmed: shapes[0] is a Circle\n";
    }

    std::cout << "\n--- Base Pointer Array ---\n";
    Shape* arr[3];
    arr[0] = shapes[0].get();
    arr[1] = shapes[1].get();
    arr[2] = shapes[2].get();

    for (int i = 0; i < 3; i++) {
        arr[i]->draw();
    }

    // -------- NEW FEATURE USAGE --------

    std::cout << "\n--- Shape Names ---\n";
    for (const auto& s : shapes) {
        std::cout << s->name() << "\n";
    }

    std::cout << "\n--- Total Area ---\n";
    std::cout << totalArea(shapes) << "\n";

    std::cout << "\n--- RTTI Demo ---\n";
    for (const auto& s : shapes) {
        printType(s.get());
    }

    // ✅ ADDED: clone demo
    std::cout << "\n--- Clone Demo ---\n";
    auto cloned = shapes[0]->clone();
    cloned->draw();

    // ✅ ADDED: smart factory
    std::cout << "\n--- Factory Demo ---\n";
    auto newShape = createShape(2);
    newShape->draw();

    // ✅ ADDED: process helper
    std::cout << "\n--- Process Shape ---\n";
    processShape(*shapes[2]);

    // ----------------------------------

    std::cout << "\n--- End of main ---\n";

    return 0;
}
