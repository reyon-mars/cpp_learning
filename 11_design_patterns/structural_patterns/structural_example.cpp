// Structural Patterns Exercise
// Adapter, Bridge, Decorator, Proxy

#include <iostream>
#include <memory>

// Adapter
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

// Decorator
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

int main() {
    auto old = std::make_shared<OldInterface>();
    Adapter adapter(old);
    adapter.new_method();
    
    auto component = std::make_shared<ConcreteComponent>();
    auto decorated = std::make_shared<ConcreteDecorator>(component);
    decorated->operation();
    
    return 0;
}
