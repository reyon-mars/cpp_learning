#include <iostream>
#include <memory>
#include <vector>
#include <string>

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

// ✅ ADDED: Another legacy system
class LegacyPrinter {
public:
    void print_legacy() { std::cout << "Legacy Printer Output\n"; }
};

// ✅ ADDED: Adapter for new interface
class PrinterAdapter : public NewInterface {
private:
    std::shared_ptr<LegacyPrinter> printer;

public:
    PrinterAdapter(std::shared_ptr<LegacyPrinter> p) : printer(p) {}

    void new_method() override {
        printer->print_legacy();
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

// ✅ ADDED: Extra decorator
class LoggingDecorator : public Decorator {
public:
    using Decorator::Decorator;

    void operation() override {
        std::cout << "[Log] Before operation\n";
        Decorator::operation();
        std::cout << "[Log] After operation\n";
    }
};

// ---------------- Bridge ----------------

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

// ✅ ADDED: Another implementation
class RasterRenderer : public Renderer {
public:
    void render_circle(float radius) override {
        std::cout << "Drawing pixels for circle of radius " << radius << "\n";
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

// ---------------- Proxy ----------------

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

// ✅ ADDED: Secure proxy with access control
class SecureImageProxy : public Image {
private:
    std::shared_ptr<RealImage> real_image;
    std::string filename;
    bool authorized;

public:
    SecureImageProxy(const std::string& file, bool auth)
        : filename(file), authorized(auth) {}

    void display() override {
        if (!authorized) {
            std::cout << "Access Denied to image: " << filename << "\n";
            return;
        }

        if (!real_image) {
            real_image = std::make_shared<RealImage>(filename);
        }
        real_image->display();
    }
};

// ---------------- Helper Functions (ADDED) ----------------

// Run multiple adapters uniformly
void run_adapters(const std::vector<std::shared_ptr<NewInterface>>& adapters) {
    for (const auto& a : adapters) {
        a->new_method();
    }
}

// Test decorator chain
void test_component(std::shared_ptr<Component> comp) {
    comp->operation();
}

// ✅ NEW: test multiple shapes
void test_shapes(const std::vector<std::shared_ptr<Shape>>& shapes) {
    for (const auto& s : shapes) {
        s->draw();
    }
}

// ✅ NEW: proxy tester
void test_proxy(Image& img) {
    std::cout << "First call:\n";
    img.display();
    std::cout << "Second call (cached):\n";
    img.display();
}

// ---------------- Main ----------------

int main() {

    // Adapter
    auto old = std::make_shared<OldInterface>();
    auto legacy = std::make_shared<LegacyPrinter>();

    std::vector<std::shared_ptr<NewInterface>> adapters;
    adapters.push_back(std::make_shared<Adapter>(old));
    adapters.push_back(std::make_shared<PrinterAdapter>(legacy));

    run_adapters(adapters);

    std::cout << "\n";

    // Decorator
    auto component = std::make_shared<ConcreteComponent>();
    auto decorated = std::make_shared<ConcreteDecorator>(component);
    auto logged = std::make_shared<LoggingDecorator>(decorated);

    test_component(logged);

    std::cout << "\n";

    // Bridge
    auto vector_renderer = std::make_shared<VectorRenderer>();
    auto raster_renderer = std::make_shared<RasterRenderer>();

    Circle c1(vector_renderer, 5.0f);
    Circle c2(raster_renderer, 3.0f);

    c1.draw();
    c2.draw();

    // ✅ NEW: batch test
    std::vector<std::shared_ptr<Shape>> shapes;
    shapes.push_back(std::make_shared<Circle>(vector_renderer, 2.0f));
    shapes.push_back(std::make_shared<Circle>(raster_renderer, 4.0f));

    std::cout << "\nBatch shape rendering:\n";
    test_shapes(shapes);

    std::cout << "\n";

    // Proxy
    ImageProxy img("photo.png");
    test_proxy(img);   // ✅ NEW helper used

    std::cout << "\n";

    // Secure proxy usage
    SecureImageProxy secure_img("secret.png", false);
    SecureImageProxy secure_img2("secret.png", true);

    secure_img.display();
    secure_img2.display();

    return 0;
}
