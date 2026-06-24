#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <string_view>
#include <numeric>
#include <algorithm>

class NewInterface {
public:
    virtual ~NewInterface() = default;
    virtual void new_method() const = 0;
};

class OldInterface {
public:
    void old_method() const { std::cout << "OldInterface::old_method\n"; }
};

class LegacyPrinter {
public:
    void print_legacy() const { std::cout << "LegacyPrinter::print_legacy\n"; }
};

class Adapter : public NewInterface {
public:
    explicit Adapter(std::shared_ptr<OldInterface> obj) : old_{std::move(obj)} {}
    void new_method() const override { old_->old_method(); }
private:
    std::shared_ptr<OldInterface> old_;
};

class PrinterAdapter : public NewInterface {
public:
    explicit PrinterAdapter(std::shared_ptr<LegacyPrinter> p) : printer_{std::move(p)} {}
    void new_method() const override { printer_->print_legacy(); }
private:
    std::shared_ptr<LegacyPrinter> printer_;
};

class Component {
public:
    virtual ~Component() = default;
    virtual void operation() const = 0;
};

class ConcreteComponent : public Component {
public:
    void operation() const override { std::cout << "ConcreteComponent::operation\n"; }
};

class Decorator : public Component {
public:
    explicit Decorator(std::shared_ptr<Component> comp) : component_{std::move(comp)} {}
    void operation() const override { component_->operation(); }
protected:
    std::shared_ptr<Component> component_;
};

class ConcreteDecorator : public Decorator {
public:
    using Decorator::Decorator;
    void operation() const override {
        std::cout << "Decorated: ";
        Decorator::operation();
    }
};

class LoggingDecorator : public Decorator {
public:
    using Decorator::Decorator;
    void operation() const override {
        std::cout << "[Log] Before\n";
        Decorator::operation();
        std::cout << "[Log] After\n";
    }
};

class Renderer {
public:
    virtual ~Renderer() = default;
    virtual void render_circle(float radius) const = 0;
};

class VectorRenderer : public Renderer {
public:
    void render_circle(float r) const override {
        std::cout << "VectorRenderer: circle radius=" << r << "\n";
    }
};

class RasterRenderer : public Renderer {
public:
    void render_circle(float r) const override {
        std::cout << "RasterRenderer: circle radius=" << r << "\n";
    }
};

class Shape {
public:
    explicit Shape(std::shared_ptr<Renderer> r) : renderer_{std::move(r)} {}
    virtual ~Shape() = default;
    virtual void draw() const = 0;
protected:
    std::shared_ptr<Renderer> renderer_;
};

class Circle : public Shape {
public:
    Circle(std::shared_ptr<Renderer> r, float radius)
        : Shape{std::move(r)}, radius_{radius} {}
    void draw() const override { renderer_->render_circle(radius_); }
    [[nodiscard]] float radius() const noexcept { return radius_; }
private:
    float radius_;
};

class Image {
public:
    virtual ~Image() = default;
    virtual void display() = 0;
};

class RealImage : public Image {
public:
    explicit RealImage(std::string_view file) : filename_{file} {
        std::cout << "Loading: " << filename_ << "\n";
    }
    void display() override { std::cout << "Displaying: " << filename_ << "\n"; }
private:
    std::string filename_;
};

class ImageProxy : public Image {
public:
    explicit ImageProxy(std::string file) : filename_{std::move(file)} {}
    void display() override {
        if (!real_) real_ = std::make_shared<RealImage>(filename_);
        real_->display();
    }
    [[nodiscard]] bool is_loaded() const noexcept { return real_ != nullptr; }
private:
    std::shared_ptr<RealImage> real_;
    std::string                filename_;
};

class SecureImageProxy : public Image {
public:
    SecureImageProxy(std::string file, bool auth)
        : filename_{std::move(file)}, authorized_{auth} {}
    void display() override {
        if (!authorized_) { std::cout << "Access denied: " << filename_ << "\n"; return; }
        if (!real_) real_ = std::make_shared<RealImage>(filename_);
        real_->display();
    }
private:
    std::shared_ptr<RealImage> real_;
    std::string                filename_;
    bool                       authorized_;
};

using ShapeVec   = std::vector<std::shared_ptr<Shape>>;
using AdapterVec = std::vector<std::shared_ptr<NewInterface>>;

void run_adapters(const AdapterVec& adapters) {
    for (const auto& a : adapters) a->new_method();
}

void test_shapes(const ShapeVec& shapes) {
    for (const auto& s : shapes) s->draw();
}

[[nodiscard]] float total_radius(const ShapeVec& shapes) {
    return std::accumulate(shapes.begin(), shapes.end(), 0.0f,
        [](float acc, const auto& s) {
            if (auto c = std::dynamic_pointer_cast<Circle>(s)) return acc + c->radius();
            return acc;
        });
}

void test_proxy(Image& img) {
    std::cout << "Call 1: "; img.display();
    std::cout << "Call 2 (cached): "; img.display();
}

int main() {
    std::cout << "=== Adapter ===\n";
    AdapterVec adapters;
    adapters.push_back(std::make_shared<Adapter>(std::make_shared<OldInterface>()));
    adapters.push_back(std::make_shared<PrinterAdapter>(std::make_shared<LegacyPrinter>()));
    run_adapters(adapters);

    std::cout << "\n=== Decorator ===\n";
    auto comp    = std::make_shared<ConcreteComponent>();
    auto deco    = std::make_shared<ConcreteDecorator>(comp);
    auto logged  = std::make_shared<LoggingDecorator>(deco);
    logged->operation();

    std::cout << "\n=== Bridge ===\n";
    auto vec_r = std::make_shared<VectorRenderer>();
    auto ras_r = std::make_shared<RasterRenderer>();
    Circle{vec_r, 5.0f}.draw();
    Circle{ras_r, 3.0f}.draw();

    ShapeVec shapes;
    shapes.push_back(std::make_shared<Circle>(vec_r, 2.0f));
    shapes.push_back(std::make_shared<Circle>(ras_r, 4.0f));
    std::cout << "Batch:\n";
    test_shapes(shapes);
    std::cout << "total_radius=" << total_radius(shapes) << "\n";

    std::cout << "\n=== Proxy ===\n";
    ImageProxy img{"photo.png"};
    test_proxy(img);
    std::cout << "is_loaded=" << std::boolalpha << img.is_loaded() << "\n";

    std::cout << "\n=== Secure Proxy ===\n";
    SecureImageProxy denied{"secret.png", false};
    SecureImageProxy granted{"secret.png", true};
    denied.display();
    granted.display();

    std::cout << "\n=== Numeric utilities ===\n";
    const std::vector<int> nums{1, 2, 3, 4, 5};
    std::cout << "sum=" << std::accumulate(nums.begin(), nums.end(), 0)
              << " max=" << *std::ranges::max_element(nums)
              << " min=" << *std::ranges::min_element(nums) << "\n";

    return 0;
}
