// Behavioral Patterns Exercise
// Observer, Strategy, Command, State

#include <iostream>
#include <vector>
#include <memory>

// Observer
class Observer {
public:
    virtual ~Observer() = default;
    virtual void update(const std::string& msg) = 0;
};

class ConcreteObserver : public Observer {
private:
    std::string name;
    
public:
    ConcreteObserver(const std::string& n) : name(n) {}
    
    void update(const std::string& msg) override {
        std::cout << name << " received: " << msg << "\n";
    }
};

class Subject {
private:
    std::vector<std::shared_ptr<Observer>> observers;
    
public:
    void attach(std::shared_ptr<Observer> obs) {
        observers.push_back(obs);
    }
    
    void notify(const std::string& msg) {
        for (auto& obs : observers) {
            obs->update(msg);
        }
    }
};

// Strategy
class Strategy {
public:
    virtual ~Strategy() = default;
    virtual int execute(int a, int b) = 0;
};

class AddStrategy : public Strategy {
public:
    int execute(int a, int b) override { return a + b; }
};

class MultiplyStrategy : public Strategy {
public:
    int execute(int a, int b) override { return a * b; }
};

int main() {
    Subject subject;
    subject.attach(std::make_shared<ConcreteObserver>("Observer1"));
    subject.attach(std::make_shared<ConcreteObserver>("Observer2"));
    subject.notify("Event occurred");
    
    return 0;
}
