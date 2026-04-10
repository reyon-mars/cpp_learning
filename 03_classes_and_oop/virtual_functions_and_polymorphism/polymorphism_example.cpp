// Virtual Functions and Polymorphism Exercise
// Dynamic dispatch, virtual functions, vtables

#include <iostream>
#include <memory>
#include <vector>
#include <typeinfo>   // ✅ added

class Shape {
public:
    Shape() { std::cout << "Shape constructed\n"; }
    virtual ~Shape() { std::cout << "Shape destroyed\n"; }

    virtual void draw() const {
        std::cout << "Drawing shape\n";
    }

    virtual double area() const = 0;

    // ----------- NEW ADDITION -----------
    virtual const char* name() const {
        return "Shape";
    }
    // -----------------------------------
};

class Circle : public Shape {
private:
    double radius;

public:
    Circle(double r) : radius(r) {
        std::cout << "Circle constructed\n";
    }

    ~Circle() {
        std::cout << "Circle destroyed\n";
    }

    void draw() const override {
        std::cout << "Drawing circle\n";
    }

    double area() const override {
        return 3.14159 * radius * radius;
    }

    // ----------- NEW ADDITION -----------
    const char* name() const override {
        return "Circle";
    }
    // -----------------------------------
};

class Rectangle : public Shape {
private:
    double width, height;

public:
    Rectangle(double w, double h) : width(w), height(h) {
        std::cout << "Rectangle constructed\n";
    }

    ~Rectangle() {
        std::cout << "Rectangle destroyed\n";
    }

    void draw() const override {
        std::cout << "Drawing rectangle\n";
    }

    double area() const override {
        return width * height;
    }

    // ----------- NEW ADDITION -----------
    const char* name() const override {
        return "Rectangle";
    }
    // -----------------------------------
};

// ----------- NEW ADDITIONS -----------

// Compute total area
double totalArea(const std::vector<std::unique_ptr<Shape>>& shapes) {
    double sum = 0;
    for (const auto& s : shapes)
        sum += s->area();
    return sum;
}

// Print runtime type info
void printType(const Shape* s) {
    std::cout << "RTTI type: " << typeid(*s).name() << "\n";
}

// ------------------------------------

int main() {

    std::vector<std::unique_ptr<Shape>> shapes;

    shapes.push_back(std::make_unique<Circle>(5.0));
    shapes.push_back(std::make_unique<Rectangle>(4.0, 6.0));

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
    Shape* arr[2];
    arr[0] = shapes[0].get();
    arr[1] = shapes[1].get();

    for (int i = 0; i < 2; i++) {
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

    // ----------------------------------

    std::cout << "\n--- End of main ---\n";

    return 0;
}
