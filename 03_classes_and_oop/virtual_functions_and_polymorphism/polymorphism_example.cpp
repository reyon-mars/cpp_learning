// Virtual Functions and Polymorphism Exercise
// Dynamic dispatch, virtual functions, vtables

#include <iostream>
#include <memory>
#include <vector>

class Shape {
public:
    Shape() { std::cout << "Shape constructed\n"; }
    virtual ~Shape() { std::cout << "Shape destroyed\n"; }

    virtual void draw() const {
        std::cout << "Drawing shape\n";
    }

    virtual double area() const = 0;
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
};

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

    std::cout << "\n--- Raw Pointer Polymorphism ---\n";   // 🔹 added
    Shape* rawPtr = shapes[1].get();
    rawPtr->draw();
    std::cout << "Area: " << rawPtr->area() << "\n";

    std::cout << "\n--- Dynamic Cast Check ---\n";   // 🔹 added
    if (Circle* c = dynamic_cast<Circle*>(shapes[0].get())) {
        std::cout << "Confirmed: shapes[0] is a Circle\n";
    }

    std::cout << "\n--- Base Pointer Array ---\n";   // 🔹 added
    Shape* arr[2];
    arr[0] = shapes[0].get();
    arr[1] = shapes[1].get();

    for (int i = 0; i < 2; i++) {
        arr[i]->draw();
    }

    std::cout << "\n--- End of main ---\n";

    return 0;
}
