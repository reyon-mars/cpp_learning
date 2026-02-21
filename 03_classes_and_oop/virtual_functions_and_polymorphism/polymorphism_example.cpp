// Virtual Functions and Polymorphism Exercise
// Dynamic dispatch, virtual functions, vtables

#include <iostream>
#include <memory>

class Shape {
public:
    virtual ~Shape() = default;
    
    virtual void draw() const {
        std::cout << "Drawing shape\n";
    }
    
    virtual double area() const = 0;
};

class Circle : public Shape {
private:
    double radius;
    
public:
    Circle(double r) : radius(r) {}
    
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
    Rectangle(double w, double h) : width(w), height(h) {}
    
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
    
    for (const auto& shape : shapes) {
        shape->draw();
        std::cout << "Area: " << shape->area() << "\n";
    }
    
    return 0;
}
