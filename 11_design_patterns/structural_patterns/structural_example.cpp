// Structural Patterns Exercise
// Adapter, Bridge, Decorator, Proxy

#include <iostream>
#include <memory>

// ---------------- Adapter ----------------

class OldInterface {
public:
    void old_method() { std::cout << "Old method\n"; }
};

class NewInterface {
public:
    virtual ~NewInterface() = default;
    virtual void new_method() = 0;
};

class Adapter : public NewInterface {
private:
    std::shared_ptr<OldInterface> old;
    
public:
    Adapter(std::shared_ptr<OldInterface> obj) : old(obj) {}
    
    void new_method() override {
        old->old_method();
    }
};

// ---------------- Decorator ----------------

class Component {
public:
    virtual ~Component() = default;
    virtual void operation() = 0;
};

class ConcreteComponent : public Component {
public:
    void operation() override { std::cout << "Operation\n"; }
};

class Decorator : public Component {
protected:
    std::shared_ptr<Component> component;
    
public:
    Decorator(std::shared_ptr<Component> comp) : component(comp) {}
    
    void operation() override {
        component->operation();
    }
};

class ConcreteDecorator : public Decorator {
public:
    using Decorator::Decorator;
    
    void operation() override {
        std::cout << "Decorated: ";
        Decorator::operation();
    }
};

// ---------------- Bridge (Small Addition) ----------------

class Renderer {
public:
    virtual ~Renderer() = default;
    virtual void render_circle(float radius) = 0;
};

class VectorRenderer : public Renderer {
public:
    void render_circle(float radius) override {
        std::cout << "Drawing circle of radius " << radius << " using vectors\n";
    }
};

class Shape {
protected:
    std::shared_ptr<Renderer> renderer;

public:
    Shape(std::shared_ptr<Renderer> r) : renderer(r) {}
    virtual ~Shape() = default;
    virtual void draw() = 0;
};

class Circle : public Shape {
private:
    float radius;

public:
    Circle(std::shared_ptr<Renderer> r, float rad)
        : Shape(r), radius(rad) {}

    void draw() override {
        renderer->render_circle(radius);
    }
};

// ---------------- Proxy (Small Addition) ----------------

class Image {
public:
    virtual ~Image() = default;
    virtual void display() = 0;
};

class RealImage : public Image {
private:
    std::string filename;

public:
    RealImage(const std::string& file) : filename(file) {
        std::cout << "Loading image: " << filename << "\n";
    }

    void display() override {
        std::cout << "Displaying image: " << filename << "\n";
    }
};

class ImageProxy : public Image {
private:
    std::shared_ptr<RealImage> real_image;
    std::string filename;

public:
    ImageProxy(const std::string& file) : filename(file) {}

    void display() override {
        if (!real_image) {
            real_image = std::make_shared<RealImage>(filename);
        }
        real_image->display();
    }
};

// ---------------- Main ----------------

int main() {

    // Adapter
    auto old = std::make_shared<OldInterface>();
    Adapter adapter(old);
    adapter.new_method();

    std::cout << "\n";

    // Decorator
    auto component = std::make_shared<ConcreteComponent>();
    auto decorated = std::make_shared<ConcreteDecorator>(component);
    decorated->operation();

    std::cout << "\n";

    // Bridge
    auto renderer = std::make_shared<VectorRenderer>();
    Circle circle(renderer, 5.0f);
    circle.draw();

    std::cout << "\n";

    // Proxy
    ImageProxy img("photo.png");
    img.display();
    img.display(); // second call uses cached object

    return 0;
}
